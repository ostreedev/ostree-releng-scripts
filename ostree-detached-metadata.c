/*
 *
 * Print/add/remove detached metadata key
 *
 * Copyright 2015 Colin Walters <walters@verbum.org>
 * Licensed under the new-BSD license (http://www.opensource.org/licenses/bsd-license.php)
*/

#include <ostree.h>

static char *opt_repo;
static char *opt_ref;
static char *opt_key;
static char *opt_set_string_file;
static gboolean opt_delete;

static GOptionEntry entries[] = {
  { "repo", 'v', 0, G_OPTION_ARG_STRING, &opt_repo, "Repo path", NULL },
  { "ref", 0, 0, G_OPTION_ARG_STRING, &opt_version, "Branch or commit", NULL },
  { "key", 0, 0, G_OPTION_ARG_STRING, &opt_version, "Metadata key", NULL },
  { "set-string-file", 0, 0, G_OPTION_ARG_STRING, &opt_version, "Path to file", NULL },
  { "delete", 0, 0, G_OPTION_ARG_NONE, &opt_delete, "Delete the key", NULL },
  { NULL }
};

int
main(int argc, char **argv)
{
  GError *real_error = NULL;
  GError **error = &real_error;
  gboolean ret = FALSE;
  GOptionContext *context;
  GFile *path;
  OstreeRepo *repo;
  char *rev;
  GVariant *current_metadata;

  context = g_option_context_new ("Main");
  g_option_context_add_main_entries (context, entries, NULL);

  if (!g_option_context_parse (context, NULL, &argc, &argv, NULL, error))
    goto out;

  path = g_file_new_for_path (opt_repo);
  repo = ostree_repo_new (path);
  if (!ostree_repo_open (repo, NULL, error))
    goto out;

  if (!ostree_repo_resolve_rev (repo, opt_ref, FALSE, &rev, error))
    goto out;

  if (strcmp (opt_ref, rev) != 0)
    g_print ("%s => %s", opt_ref, rev);

  if (!ostree_repo_read_commit_detached_metadata (repo, rev, &current_metadata, NULL, error))
    goto out;

  if (opt_key == NULL)
    {
      GVariantIter viter;
      const char *key;

      g_variant_iter_init (&viter, &current_metadata);
      while (g_variant_iter_loop (&viter, "{&s@v}",
				  &key, NULL))
	g_print ("%s\n", key);

      goto out;
    }
  else
    {
      GVariantBuilder vbuilder;
      GVariantIter viter;
      const char *key;

      g_variant_builder_init (&vbuiler, "a{sv}");

      if (opt_delete)
	{
	  GVariant *val;
	  g_variant_iter_init (&viter, &current_metadata);
	  while (g_variant_iter_loop (&viter, "{&s@v}",
				      &key, &val))
	    {
	      if (strcmp (key, opt_key) == 0)
		continue;
	      g_variant_builder_add (&vbuilder, "{s@v}", key, val);
	    }
	}
      else
	{
	  GVariant *val;
	  g_variant_iter_init (&viter, &current_metadata);
	  while (g_variant_iter_loop (&viter, "{&s@v}",
				      &key, &val))
	    {
	      g_variant_builder_add (&vbuilder, "{s@v}", key, val);
	    }
	}
    }

 out:
  if (real_error)
    {
      g_printerr ("%s\n", real_error->message);
      return 1;
    }
  return 0;
}
arg = parser.parse_args()

r = OSTree.Repo.new(Gio.File.new_for_path(arg.repo))
r.open(None)

[_,rev] = r.resolve_rev(arg.ref, False)

if arg.ref != rev:
if metadata is None:
    metadata = new_metadata = {}
else:
    new_metadata = {}

arg = parser.parse_args()

if arg.key is None:
    keys = metadata.keys()
    if len(keys) == 0:
        print "No metadata."
    else:
        for k in keys:
            print k
elif arg.delete:
    for k in metadata.keys():
      for 
        if k == arg.key:
            continue
        new_metadata[k] = GLib.Variant("v", metadata[k])
    new_metadatav = GLib.Variant('a{sv}', new_metadata)
    r.write_commit_detached_metadata(rev, new_metadatav, None)
    print "Deleted key: %s" % (arg.key, )
elif arg.set_string_file is not None:
    for k in metadata.keys():
        new_metadata[k] = GLib.Variant("v", metadata[k])
    with open(arg.set_string_file) as f:
        data = f.read()
        new_metadata[arg.key] = GLib.Variant('s', data)
    new_metadatav = GLib.Variant('a{sv}', new_metadata)
    r.write_commit_detached_metadata(rev, new_metadatav, None)
    print "Wrote key: %s" % (arg.key, )
else:
    print metadata[arg.key]

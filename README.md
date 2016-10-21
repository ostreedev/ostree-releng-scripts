Scripts for release engineering operations on OSTree repositories
-----------------------------------------------------------------

These scripts are intended for use by organizations that provide
OSTree repositories, automating workflow operations around
release engineering.

## do-release-tags

This implements a "binary promotion model" of commits across separate
branches, similar to what's described in [the ostree manual](https://ostree.readthedocs.io/en/latest/manual/repository-management/#promoting-content-along-ostree-branches-buildmaster-smoketested).
This script is used by [CentOS Atomic Host Development](https://wiki.centos.org/SpecialInterestGroup/Atomic/Devel).
See the [releases.yml](https://github.com/CentOS/sig-atomic-buildscripts/blob/master/releases.yml) file there.

To use this, you should store the `releases.yml` file in git, and ensure that
`do-release-tags` is executed from a CI system like Jenkins.

## reset-if-staging

A simpler pattern is to have an internal repository which has at
most one new commit, which isn't exposed publicly.  This script
maintains a `redhat.staging` detached metadata key, and will
undo the topmost commit if it has that key.

To use this, just run it before your build system generates a commit.

## rsync-repos

While you certainly can invoke rsync against a whole OSTree repo,
there will be race conditions if any client systems are accessing it
concurrently.  This script wraps rsync to close those races.

Note that if you have concurrent writes on the repo, and particularly
if you want to do pruning, a best practice for using this is to use ostree to
create a local snapshot.  Like this:

```
ostree --repo=repo.tmp init --mode=archive
ostree --repo=repo.tmp pull-local repo
ostree --repo=repo.tmp prune --refs-only --keep-younger-than='30 days ago'
ostree --repo=repo.tmp summary -u
./rsync-repos repo.tmp/ contentserver.example.com:/srv/repo/
rm repo.tmp -rf
```

Since with this model the prunes are only happening in the public copy,
you'll want to periodically halt writes to the internal copy, and
reverse the rsync direction.

## print-commitsize

Generate statistics on the files in a given commit.

## write-ref

Directly change a given ref, with no verification.

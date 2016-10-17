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

## print-commitsize

Generate statistics on the files in a given commit.

## write-ref

Directly change a given ref, with no verification.

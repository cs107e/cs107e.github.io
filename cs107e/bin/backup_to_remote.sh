#!/bin/bash
# jzelenski & mchang Apr 2026
# script to backup changed source files to remote
# using separate branch and worktree so as to
# not muck with working dir/main git

# arg -v for verbose (turns off quiet)
QUIET="--quiet"
[ "$1" = "-v" ] && QUIET=""

if ! git rev-parse --git-dir > /dev/null 2>&1; then
    echo "$(basename "$0"): Error: Not in a git repository"
    exit 1
fi

REPO_ROOT=$(git rev-parse --show-toplevel)
SUBDIR=$(git rev-parse --show-prefix)        # subdir within repo root for cwd

if [ -n "$CS107E_STUDENTS_DIR" ] ; then
    exit # skip if this is staff member in student repo
fi

# find existing backup worktree or create new
BRANCH_NAME="auto-backup"
WORKTREE_PATH=$(git worktree list | grep -F "[$BRANCH_NAME]" | cut -f1 -d' ')
if [ -z "$WORKTREE_PATH" ]; then # if no worktree exists, create one
    WORKTREE_PATH="$REPO_ROOT/.backup_worktree"
    # worktree add orphan avail git version >= 2.42, if fails, do manually
    if ! git worktree add $QUIET -b $BRANCH_NAME --orphan $WORKTREE_PATH 2>/dev/null; then
        git worktree add $QUIET --detach --no-checkout $WORKTREE_PATH
        git -C $WORKTREE_PATH checkout $QUIET --orphan $BRANCH_NAME
    fi
    git -C $WORKTREE_PATH commit $QUIET --allow-empty -m "Init branch $BRANCH_NAME"
    git -C $WORKTREE_PATH pull $QUIET --no-rebase --no-edit --allow-unrelated-histories --strategy=ours origin $BRANCH_NAME 2>/dev/null
    git -C $WORKTREE_PATH push $QUIET --set-upstream origin $BRANCH_NAME 2>/dev/null
fi

shopt -s nullglob          # set non-match glob expands to empty
SOURCES=$(echo *.c *.s)    # gather source files from cwd
if [ -n "$SOURCES" ]; then
    git -C $WORKTREE_PATH pull $QUIET --no-rebase --no-edit --allow-unrelated-histories --strategy=ours origin $BRANCH_NAME 2>/dev/null
    mkdir -p $WORKTREE_PATH/$SUBDIR         # ensure subdir exists
    cp -f *.c *.s $WORKTREE_PATH/$SUBDIR  2>/dev/null  # copy sources to subdir in backup worktree
    git -C $WORKTREE_PATH add .             # add to staging
    if ! git -C $WORKTREE_PATH diff --quiet --staged; then
        # non-empty changes are staged, commit & push
        git -C $WORKTREE_PATH commit $QUIET -m "Auto-backup $(date '+%Y-%m-%d %H:%M:%S')"
        git -C $WORKTREE_PATH push $QUIET origin $BRANCH_NAME
    fi
fi
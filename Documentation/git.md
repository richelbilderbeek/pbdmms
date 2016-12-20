# git and GitHub

For those unfamiliar with git and GitHub, here a summary of

 * the normal workflow, with `clone`, `add`, `commit`, `push`, `pull`
 * the braching model, with `checkout` and `merge`
 * useful tricks

## Workflow

Here it is:

```
git clone https://github.com/richelbilderbeek/pbdmms
git add --all :/
git commit -m "Did something"
git push
git pull
```

More detail below.

### `clone`

To `clone` a is to downloads a GitHub repository to 
a local hard drive. This wil be the first thing you'll do.

```
git clone https://github.com/richelbilderbeek/pbdmms
```

### `add` and `commit`

To `add` is to collect/stage which files are changed.
To `commit` denotes naming the changes made to the `add`ed/staged files.

```
git add --all :/
git commit -m "Did something"
```

### `push`

To `push` is to upload your changed code to GitHub.

```
git push
```

Consider to push at the end of the day


### `pull`

To `pull` is to update your local code with 
possible changes made to the GitHub.

```
git pull
```

Consider to pull at theh start of the day


## Branching model

We follow the same structure as [here](http://nvie.com/posts/a-successful-git-branching-model/):

 * `master`: the stable build, should always pass
 * `[namespace name]`: a developer's branch

Load the current content of a branch, for example `ribi`:

```
git checkout ribi
```

Merge code from `master`:

```
git merge master
```

Nearly always followed by:

```
git push
```

If your branch builds correctly, do:

Load the master branch:

```
git checkout master
```

Merge code from your development branch:

```
git merge ribi
```

Nearly always followed by:

```
git push
```

If this breaks this build, people will not
be happy. Tip: bring them tea/coffee as long as the
`master` is broken

## Useful tricks

Change password once per ten hours on a computer:

```
git config --global credential.helper cache
git config --global credential.helper 'cache --timeout=36000'
```

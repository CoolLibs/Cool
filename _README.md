# One or many repos?

## One repo for all the Cool modules

All modules are always up-to-date with one another. Since users have only one repo to pull from, this is always a working version (unlike having many repos, each one relying on a specific version of the other ones).

Changing the API only requires one single Rename operation on the whole project.

## One repo for each Cool module

Users can choose which ones they want to download and which ones they don't need.

Encourages more decoupling between the modules.
# SatalinOS Utility

This program serves as an assistant utility for [SatalinOS](https://github.com/IronLeoo/satalin-os).
It provides functionality for installing, reinstalling and updating packages and the OS itself.
It comes preinstalled with SatalinOS 2.6 and above.

## Installation

1. Download repo
```
git clone https://github.com/IronLeoo/satalin-os-utility.git /usr/src/satalin-os-utility
```
2. Build binary
```
/usr/src/build.sh
```
3. Install
```
ln -s /usr/src/satalin-os-utility/sosu /usr/sbin/sosu
```

## Usage

`sosu [install|reinstall|update] <parameters>`

### Install Module

To install pre-defined software packages use the 'install' module and specify the targets.<br>
This module accepts multiple targets at once.<br>
Example: `sosu install base laptop` to install the base SatalinOS packages and the additional packages for laptop users.

### Reinstall Module

To reinstall all software packages run `sosu reinstall packages`.<br>
To reinstall the config files run `sosu reinstall configs`.<br>
To reinstall sosu itself run `sosu reinstall sosu`.<br>
To reinstall all of the above run `sosu reinstall all`.

### Update Module

To update all software packages run `sosu update packages`.<br>
To update the config files run `sosu update configs`.<br>
To update sosu itself run `sosu update sosu`.<br>
To update all of the above run `sosu update all`.

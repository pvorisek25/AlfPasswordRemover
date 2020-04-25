# AlfPasswordRemover

As the name states, AlfPasswordRemover can remove passwords from [Alf](https://programalf.com/alf/en/index.html) test files. It uses the [ZipLib](https://bitbucket.org/wbenny/ziplib/src/master/) library for extracting test files.

## Download
You can download pre-built binaries of AlfPasswordRemover for Windows and Linux here: https://github.com/pvorisek25/AlfPasswordRemover/releases.

## Building
### Windows
In order to build APR on Windows you'll need [Visual Studio](https://visualstudio.microsoft.com/vs/). After installing Visual Studio and rebooting your computer go to the APR's root folder and run the ``generate.bat`` script. This should generate Visual Studio solution file. Open this file in Visual Studio and build the program.

### Linux
On Linux you'll need to install the ``build-essential`` package. If you're on Debian-based distributions, run ``sudo apt-get install build-essential`` in the Terminal. After installing the package go to the APR's root folder and run the ``generate.sh`` script. The script should generate [Makefile](https://www.gnu.org/software/make/) files. Run ``make help`` to see all avaiable configurations and run ``make config=[your configuration]`` to build APR.

## Usage
APR is a console application so you have to open the Command Prompt or Terminal to execute it. On Windows, simply enter ``AlfPasswordRemover [testfile].alf``. On Linux and other POSIX systems enter ``sudo ./AlfPasswordRemover [testfile].alf``.

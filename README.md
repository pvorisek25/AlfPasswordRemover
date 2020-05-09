# AlfPasswordRemover

As the name states, AlfPasswordRemover (APR) can remove passwords from [Alf](https://programalf.com/alf/en/index.html) test files. It uses the [ZipLib](https://bitbucket.org/wbenny/ziplib/src/master/) library for extracting test files.

## Download
You can download pre-built binaries of AlfPasswordRemover for Windows and Linux here: https://github.com/pvorisek25/AlfPasswordRemover/releases.

## Building
### Windows
In order to build APR on Windows you'll need [Visual Studio](https://visualstudio.microsoft.com/vs/). After installing Visual Studio and rebooting your computer, go to the APR's root folder and run the ``generate.bat`` script. This should generate Visual Studio solution file. Open this file in Visual Studio and build the program. The APR's binaries will be located in ``[APR's root folder]/bin/[your configuration]-[your processor's architecture]-windows``.

### Linux
On Linux you'll need to install the ``build-essential`` package. If you're on Debian-based distributions, run ``sudo apt-get install build-essential`` in the Terminal. After installing the package, go to the APR's root folder and run the ``generate.sh`` script. The script should generate [GNU Make](https://www.gnu.org/software/make/) files. Run ``make help`` to see all available configurations and run ``make config=[your configuration]`` to build APR. The APR's binaries will be located in ``[APR's root folder]/bin/[your configuration]-[your processor's architecture]-linux``.

### Other POSIX systems
On other POSIX systems you'll need to install [GCC](https://gcc.gnu.org/) and [GNU Make](https://www.gnu.org/software/make/) on your computer. Once installed, go to the ``premake`` folder and unzip the source code of Premake (``premake-5.0.0-alpha14-src.zip``). After unzipping the archive, go to ``[Premake's root folder]/build/gmake2.unix`` and run ``gmake config=release``. The binaries should be located in ``[Premake's root folder]/bin/release``. Copy the binaries from the ``release`` folder to the APR's root folder. In the APR's root folder run ``[name of the Premake binaries] gmake``. This will generate [GNU Make](https://www.gnu.org/software/make/) files. Run ``gmake help`` to see all available configurations and run ``gmake config=[your configuration]`` to build APR. The APR's binaries will be located in ``[APR's root folder]/bin/[your configuration]-[your processor's architecture]-[your OS]``.

## Usage
APR is a console application so you have to open the Command Prompt or Terminal to execute it. Run ``AlfPasswordRemover.exe [yourtest].alf`` on Windows and ``./AlfPasswordRemover [yourtest].alf`` on Linux and POSIX systems. Don't forget to run APR with root priviliges on Linux and POSIX systems. 

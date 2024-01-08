[slideNormalize](https://github.com/swvanderlaan/slideNormalize)
============

**slideNormalize**: normalisation of histological high-resolution images.

We have tested **slideNormalize** on CentOS7 and macOS Mojave+ (version 10.14.5+).

Please tell us if you run into problems, it is likely we can help you out - we have done this before. ;)


--------------

#### Installation 
Download and install the latest version of the **slideNormalize** from GitHub. First create and go to the git directory, then download **slideNormalize**.

```
mkdir -p ~/git/ && cd ~/git
```
```
if [ -d ~/git/slideNormalize/.git ]; then \
		cd ~/git/slideNormalize && git pull; \
	else \
		cd ~/git/ && git clone https://github.com/swvanderlaan/slideNormalize.git; \
	fi
```

#### Binaries

For your convenience, we have created precompiled binaries for CentOS7+ and macOS Mojave (10.14.5+) using `g++` and `opencv2`. These are available in the `binaries` folder:

- `slideNormalize_macOS_Mojave10145`
- `slideNormalize_macOS_BigSur1152`
- `slideNormalize_centos7`

#### Compile from source

**slideNormalize** requires [`opencv2`](https://formulae.brew.sh/formula/opencv@2) which you can install via [`brew`](https://brew.sh).

```
brew install opencv@2
```

It will probably be keg-only, so you need to make sure that `opencv2` is in your `$PKG_CONFIG`. Add this to your `.bashrc` or `.bash_profile`

```
echo 'export PKG_CONFIG_PATH="/usr/local/opt/opencv@2/lib/pkgconfig:$PKG_CONFIG_PATH"' >> ~/.bashrc
```

#### slideNormalize in your path
You can add **slideNormalize** to your path by running the code below to make a symbolic link, while making sure to choose the binary appropriate for you system (macOS or CentOS).

```
mkdir -pv ~/bin/ && \
	ln -s -f -v ~/git/slideNormalize/binaries/slideNormalize_mac ~/bin/slideNormalize
```

#### Example usage

The program uses the `TCLAP` library for command-line argument parsing. Here is the list of command-line arguments:

**Argument(s):**
- **`-f` or `--file`**
  - The filename of the NDPI/TIF file to process. (Example: `-f input_image.tif`)

**Optional argument(s):**
- **`-o` or `--output`**
  - The output filename of the NDPI/TIF file to process. If not provided, a default filename will be generated based on the input filename. (Example: `-o output_image.tif`)
- **`-e` or `--extension`**
  - The standard output filename extension of the image-tile to process. If provided, it will be used in the generated output filename. (Example: `-e normalized`)
- **`-s` or `--show`**
  - Show results in a graphical interface. This is a switch, and if included, it will trigger the display of results. (Example: `-s`)

Here's an example of how you might run the program from the command line:

```
./slideNormalize -f input_image.tif -o output_image.tif -e normalized -s
```

The `-f` flag specifies the input file, `-o` specifies the output file, `-e` specifies the extension, and `-s` triggers the graphical display of results.


-----------------------------------------------
#### Licence
The MIT License (MIT): <http://opensource.org/licenses/MIT>.

Copyright (c) 2017-2024, [Tim Bezemer](https://github.com/tbezemer) & [Sander W. van der Laan](https://github.com/swvanderlaan), UMC Utrecht, Utrecht, the Netherlands.


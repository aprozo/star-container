# Star Tutorial

This is an example analysis of [`StPicoDst`](https://www.star.bnl.gov/webdata/dox/html/classStPicoDstMaker.html):
- `St` stands for STAR, [all classes](https://www.star.bnl.gov/webdata/dox/html/annotated.html) start from it 
- `Pico` means the tree data format where only small number of numerous qualities are saved, exist also `Micro` and `Mini`,
- `Dst` means Data Summary Table)


It is based on [Grigory's presentation on PicoDst 2019](https://drupal.star.bnl.gov/STAR/system/files/Nigmatkulov_intro2pico_Krakow2019.pdf)

## Instructions to run

The star-sw version is [here](https://github.com/star-bnl/star-sw), which corresponds to the latest version `star pro` with compiler `gcc4.85` and `ROOT 5.34` version.

STAR uses [`cons` command](https://www.gnu.org/software/cons/stable/cons.html) to compile. This is similar to compiling projects using `Make` file or performing `g++` command within complex `C++` projects (whic STAR is).
The `cons` command will:
- create a directory `.sl7X_gccXX` where all compiled libraries will go which are used in your project
- perform compilation of all your files with `.cxx`(important!) and connected headers `.h` under `StRoot` directory.

 



### Some old presentation on DST tutorials:

- [Introduction to PicoDst](https://drupal.star.bnl.gov/STAR/system/files/Nigmatkulov_intro2pico_Krakow2019.pdf) (Grigory Nigmatkulov, 2019)
- Introduction to MuDst:
  - [Starting Data Analysis on STAR](http://nuclear.ucdavis.edu/~brovko/GettingStarted.pdf) (Samantha Brovko, 2011)
  - [A common-MuDst tutorial](https://www.star.bnl.gov/public/comp/meet/RM200311/MuDstTutorial.pdf) (Sergey Panitkin, 2003)


# Configuración del Entorno de Desarrollo para xv6 en Mac con Apple Silicon

## 1. Introducción

Este informe detalla los pasos seguidos para la instalación y configuración del entorno de desarrollo de xv6, en un equipo con procesador Apple Silicon (M2). Se incluyen las instrucciones para clonar el repositorio, crear una nueva rama de trabajo, instalar el toolchain de RISC-V utilizando Homebrew y verificar la correcta instalación del sistema.

## 2. Pasos Realizados

### 2.1. Fork del Repositorio Original de MIT

El primer paso consistió en realizar un fork del repositorio oficial de xv6 alojado en el MIT. Esto permite trabajar en una copia personal del proyecto:

- **Repositorio original**: [MIT xv6 Repository](https://github.com/mit-pdos/xv6-riscv).


### 2.2. Clonado del Repositorio en el Entorno Local

Una vez realizado el fork, se procedió a clonar el repositorio en el entorno local de la máquina:

### Tercer paso 
Crear una branch con `$ git checkout -b gabrielcaviedes-tarea-0`

### Cuarto paso
Instalar risc-v toolchain con homebrew con este repo: https://github.com/riscv-software-src/homebrew-riscv

### Quinto paso:
Ejecutar `$ brew tap riscv-software-src/riscv`

### Sexto paso: 
Ejecutar arch -arm64 brew install riscv-tools, en mi caso como tengo un Apple Silicon M2 lo hice con esto. En otro caso se hacía con este: $ brew install riscv-tools

# Verificamos instalación

### Hacemos un `make clean` para luego proceder a un `make qemu`

### Tenemos esto al hacer las pruebas: 

```sh
xv6 kernel is booting

hart 1 starting
hart 2 starting
init: starting sh
$ ls
.              1 1 1024
..             1 1 1024
README         2 2 2403
cat            2 3 35592
echo           2 4 34448
forktest       2 5 16264
grep           2 6 39040
init           2 7 34904
kill           2 8 34368
ln             2 9 34184
ls             2 10 37712
mkdir          2 11 34432
rm             2 12 34416
sh             2 13 57160
stressfs       2 14 35304
usertests      2 15 181208
grind          2 16 50872
wc             2 17 36584
zombie         2 18 33768
console        3 19 0
$ echo "Hola xv6"
"Hola xv6"
$ cat README
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern RISC-V multiprocessor using ANSI C.

ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)).  See also https://pdos.csail.mit.edu/6.1810/, which provides
pointers to on-line resources for v6.

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by
Takahiro Aoyagi, Marcelo Arroyo, Silas Boyd-Wickizer, Anton Burtsev,
carlclone, Ian Chen, Dan Cross, Cody Cutler, Mike CAT, Tej Chajed,
Asami Doi,Wenyang Duan, eyalz800, Nelson Elhage, Saar Ettinger, Alice
Ferrazzi, Nathaniel Filardo, flespark, Peter Froehlich, Yakir Goaron,
Shivam Handa, Matt Harvey, Bryan Henry, jaichenhengjie, Jim Huang,
Matúš Jókay, John Jolly, Alexander Kapshuk, Anders Kaseorg, kehao95,
Wolfgang Keller, Jungwoo Kim, Jonathan Kimmitt, Eddie Kohler, Vadim
Kolontsov, Austin Liew, l0stman, Pavan Maddamsetti, Imbar Marinescu,
Yandong Mao, Matan Shabtay, Hitoshi Mitake, Carmi Merimovich, Mark
Morrissey, mtasm, Joel Nider, Hayato Ohhashi, OptimisticSide,
phosphagos, Harry Porter, Greg Price, RayAndrew, Jude Rich, segfault,
Ayan Shafqat, Eldar Sehayek, Yongming Shen, Fumiya Shigemitsu, snoire,
Taojie, Cam Tenny, tyfkda, Warren Toomey, Stephen Tu, Alissa Tung,
Rafael Ubal, Amane Uehara, Pablo Ventura, Xi Wang, WaheedHafez,
Keiichi Watanabe, Lucas Wolf, Nicolas Wolovick, wxdao, Grant Wu, x653,
Jindong Zhang, Icenowy Zheng, ZhUyU1997, and Zou Chang Wei.

The code in the files that constitute xv6 is
Copyright 2006-2024 Frans Kaashoek, Robert Morris, and Russ Cox.

ERROR REPORTS

Please send errors and suggestions to Frans Kaashoek and Robert Morris
(kaashoek,rtm@mit.edu).  The main purpose of xv6 is as a teaching
operating system for MIT's 6.1810, so we are more interested in
simplifications and clarifications than new features.

BUILDING AND RUNNING XV6

You will need a RISC-V "newlib" tool chain from
https://github.com/riscv/riscv-gnu-toolchain, and qemu compiled for
riscv64-softmmu.  Once they are installed, and in your shell
search path, you can run "make qemu".
```
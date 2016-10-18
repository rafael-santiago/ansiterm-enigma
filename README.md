# ANSI/TERM Enigma

## What is this?

It is an ``Enigma machine`` simulator for ``ANSI/TERM`` environments. The simulator
is based on ``M3 Enigma``.

**WARNING**: This software is merely a *toy* do not use it to protect your data.

Basically you have got this simple "term/geek" interface (link your viewer with ``-limagination``):

![Sample](https://github.com/rafael-santiago/ansiterm-enigma/blob/master/etc/sample.gif)

## How to clone it?

This repository has some submodules so the simplest way of clonning it is:

```
monster@CraterLake:~# git clone https://github.com/rafael-santiago/ansiterm-enigma --recursive
```

## How to build it?

In order to build this project you need [Hefesto](https://github.com/rafael-santiago/hefesto). Once ``Hefesto`` installed
and working on your system for building this simulator, supposing that you cloned it into ``ansiterm-enigma`` sub-directory,
you should proceed as follows:

```
monster@CraterLake:~# cd ansiterm-enigma/src
monster@CraterLake:~/ansiterm-enigma/src# hefesto
```

## How to install it?

Being under the ``src`` sub-directory:

```
monster@CraterLake:~/ansiterm-enigma/src# hefesto --install
```

It also installs a tiny ``man page`` that you can use as future command reference/cheat sheet.

For uninstalling issues you should use ``hefesto --uninstall``.

**Tip**: if you are on ``FreeBSD`` maybe you should have installed a previous application also called "enigma", in this case
in order to access this ``enigma`` simulator (after installing) you should explicitly call ``/usr/local/bin/enigma``.

## How to use it?

You need to pass through command line seven mandatory options to this simulator:

- The option ``--reflector`` to indicate the used reflector. It can be: ``b`` or ``c``.
- The option ``--l-rotor`` to indicate what rotor to use on the left. It can be: ``i``, ``ii``, ``iii``, ``iv``, ``v``, ``vi``, ``vii`` or ``viii``.
- The option ``--m-rotor`` to indicate what rotor to use at the middle. It can be: ``i``, ``ii``, ``iii``, ``iv``, ``v``, ``vi``, ``vii`` or ``viii``.
- The option ``--r-rotor`` to indicate what rotor to use on the right. It can be: ``i``, ``ii``, ``iii``, ``iv``, ``v``, ``vi``, ``vii`` or ``viii``.
- The option ``--l-rotor-at`` to indicate the initial position of the ``l-rotor``. It can be a letter between ``A`` and ``Z``.
- The option ``--m-rotor-at`` to indicate the initial position of the ``m-rotor``. It can be a letter between ``A`` and ``Z``.
- The option ``--r-rotor-at`` to indicate the initial position of the ``r-rotor``. It can be a letter between ``A`` and ``Z``.

Also there are four optional settings:

- The option ``--l-ring`` which specifies the ring offset of the left rotor. It can be a value between ``1`` and ``26``. The default is ``1``.
- The option ``--m-ring`` which specifies the ring offset of the middle rotor. It can be a value between ``1`` and ``26``. The default is ``1``.
- The option ``--r-ring`` which specifies the ring offset of the right rotor. It can be a value between ``1`` and ``26``. The default is ``1``.
- The option ``--plugboard`` which can specifies until ten swap pairs in form ``<letterX>/<letterY>,...,<letterX'>/<letterY'>``. No swaps are assumed by default.

Follows an example:

```
monster@CraterLake:~# enigma --reflector=c\
> --l-rotor=iv --l-rotor-at=y\
> --m-rotor=vii --m-rotor-at=s\
> --r-rotor=ii --r-rotor-at=z\
> --plugboard=A/S,D/F,M/N,C/X
```

You need to press ``ESC`` to exit.

Enjoy!

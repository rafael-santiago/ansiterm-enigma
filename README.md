# ansiterm-enigma

## What is this?

It is an ``Enigma machine`` simulator for ``ANSI/TERM`` environments. The simulator
is based on ``M3 Enigma``.

Basically you got this simple interface:

## How to clone it?

This repository has some submodules so the simplest way of clonning it is:

```
monster@CraterLake:~/# git clone https://github.com/rafael-santiago/ansiterm-enigma --recursive
```

## How to build it?

In order to build this project you need [Hefesto](https://github.com/rafael-santiago/hefesto). Once ``Hefesto`` installed
and working on your system for building this simulator, supposing that you cloned it into ``ansiterm-enigma`` sub-directory,
you should proceed as follows:

```
monster@CraterLake:~/# cd ansiterm-enigma/src
monster@CraterLake:~/ansiterm-enigma/src# hefesto
```

## How to install it?

Being under the ``src`` sub-directory:

```
monster@CraterLake:~/ansiter-enigma/src# hefesto --install
```

It also installs a tiny ``man page`` that you can use as future command reference.

For uninstalling is ``hefesto --uninstall``.

## How to use it?

For using this simulator you need to pass through command line seven mandatory options:

- The option ``--reflector`` to indicate the used reflector. It can be: ``b`` or ``c``.
- The option ``--l-rotor`` to indicate what rotor to use on the left. It can be: ``i``, ``ii``, ``iii``, ``iv``, ``v``, ``vi``, ``vii`` or ``viii``.
- The option ``--m-rotor`` to indicate what rotor to use at the middle. It can be: ``i``, ``ii``, ``iii``, ``iv``, ``v``, ``vi``, ``vii`` or ``viii``.
- The option ``--r-rotor`` to indicate what rotor to use on the right. It can be: ``i``, ``ii``, ``iii``, ``iv``, ``v``, ``vi``, ``vii`` or ``viii``.
- The option ``--l-rotor-at`` to indicate the initial position of the ``l-rotor``. It can be a letter between ``A`` and ``Z``.
- The option ``--m-rotor-at`` to indicate the initial position of the ``m-rotor``. It can be a letter between ``A`` and ``Z``.
- The option ``--r-rotor-at`` to indicate the initial position of the ``r-rotor``. It can be a letter between ``A`` and ``Z``.

Also there are three optional settings:

- The option ``--l-ring`` which specifies the ring offset of the left rotor. It can be a value between ``1`` and ``26``. The default is ``1``.
- The option ``--m-ring`` which specifies the ring offset of the middle rotor. It can be a value between ``1`` and ``26``. The default is ``1``.
- The option ``--r-ring`` which specifies the ring offset of the right rotor. It can be a value between ``1`` and ``26``. The default is ``1``.

Follows an example:

```
root@CraterLake:~/# enigma --reflector=c\
> --l-rotor=iv --l-rotor-at=y\
> --m-rotor=vii --m-rotor-at=s\
> --r-rotor=ii --r-rotor-at=z
```

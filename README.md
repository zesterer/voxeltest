# VoxelTest

A test voxel engine program

## What is VoxelTest?

VoxelTest is simply a test program I use when developing my voxel library, 'libvolume'.

Compile and run it if you so wish, but it's not very interesting unless, like me, you have the strange sort of brain that feels satisfied when you see lots of text flying up the display of a terminal emulator without any occurence of the word 'error'. Although, in reality, you're quite likely to get several of them.

## Downloading

~ First, navigate to an appropriate directory ~

`git clone https://github.com/zesterer/voxeltest`

`cd voxeltest`

`rm libvolume`

`git clone https://github.com/zesterer/libvolume`

## Compilation

~ From the root repository directory, as already done in the previous step ~

`mkdir build`

`cd build`

`cmake ..`

`make`

## Running

~ Assuming you've run the compilation commands above ~

`./voxeltest`

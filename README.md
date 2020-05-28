# CS470 Lab2

## Introduction

This README is for a program that takes a text file as a command line argument and outputs the frequency of all letters Aa-Zz.

## Requirements

This program is meant to be ran on a **macOS** system.

## Instructions

To run this program, open a terminal and navigate to the directory where the program exists. After compiling main.c on a macOS system, run the program from the command line with a valid path to a text file that you wish to parse. e.g *./a.out TextFile.txt*

## Error Handling

This program will exit if an invalid or missing file path is given.

Once a valid file is opened, the program will prompt the user for confirmation to parse the file. 
Response should be given with either 'y' or 'n'. All other inputs will be rejected and the user will be requested to re-enter their response. 


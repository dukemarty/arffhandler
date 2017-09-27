# arffhandler

Simple library for handling and manipulating ARFF files.

## Overview

Currently, the most convenient way to manipulate ARFF files is via [WEKA](http://www.cs.waikato.ac.nz/ml/weka/). But if using ARFF files in a C/C++ environment, it would of course be more convenient to have a handy library available for that.
This is what this project provides.

## Vision

In the future, it will be super-easy to remove columns or rows or to merge them. Using a GUI as well as directly from the command line.

## Architecture: Summary of main components and their relation.

### Main components

The main components that make up the _arffhandler_ library are:

   * `Containers` :  different container types for holding large set of (raw) data
   * `ARFFData` :  class representation of ARFF data, holding the raw data as well as meta information like feature lists etc.
   * `ARFFFileHandler` :  public interface for interacting with ARFF data


### Interactions and relations

The components are connected in the following way(s):

  * `ARFFData <-> Containers` :  Containers are allocated, filled and manipulated by ARFFData, thereby used to store the main contents of ARFF files.
  * `ARFFFileHandler <-> ARFFData` :  The interface provided by ARFFFileHandler uses internally an ARFFData object for each ARFF file it is interacting with.
 

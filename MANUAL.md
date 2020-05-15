# crnsimul - Manual
This manual will cover the necessary information and instructions to operate crnsimul, a program for simulation of chemical reaction networks (CRN). The crnsimul program is programmed in the C++ programming language, and generated using the tools [FLEX](https://github.com/westes/flex/) and [BISON](https://github.com/westes/flex/). 
The crnsimul program was, alongside [chemilang](https://github.com/RasmusRendal/chemilang), developed under the same project by a group of university students from AAU.
The crnsimul program contains features such as real time plotting of CRNs, which will be further elaborated on later in this manual.

#### Prerequisites
This manual assumes that you have installed crnsimul, and have it in your `$PATH`.

To compile and install crnsimul, check the [README](https://github.com/RasmusRendal/crnsimul/blob/milestone/1.0/README.md).

## Table of Content

- [crnsimul - Manual](#crnsimul---manual)
      - [Prerequisites](#prerequisites)
  - [Table of Content](#table-of-content)
    - [1. Writing simple chemical reaction networks](#1-writing-simple-chemical-reaction-networks)
    - [2. Plotting of CRNs](#2-plotting-of-crns)
      - [2.1 OpenRTPlotter](#21-openrtplotter)
      - [2.2 GNU plotting](#22-gnu-plotting)
      - [3. Printing to .csv files](#3-printing-to-csv-files)
    - [4. Evaluators](#4-evaluators)
      - [4.1 Euler evaluator](#41-Euler-evaluator)
      - [4.2 Markov evaluator](#42-markov-evaluator)
    - [5. Stdout](#5-stdout)
<br />

### 1. Writing simple chemical reaction networks
The crnsimul takes input files formatted as CRNs. It is convention to give these files the **".crn** extension, but the program does not care.

If you are deveoping your CRN for use in a UNIX-like system, it can be helpful to add a shebang and mark it as executable, to allow executing the file directly. The recommended shebang is of the format:
``` c++
#!/usr/bin/env -S crnsimul -RTP 
```
<br />

The following code below is an example of a CRN module that follows the implementation of adding two species together, namely species **a** and **b**. 

``` c++
#!/usr/bin/env -S crnsimul -P 

a := 10;
b := 5;

a -> x;
b -> x;
x -> 0;
```
Here, the first two lines following the UNIX shebang are assigned concentrations whereas the rest of the CRN module shows the chemical reactions.
Various examples of CRN modules can be found in the examples folder within the crnsimul repository. 

### 2. Plotting of CRNs 
The main feature of the crnsimul program is the ability to have real time plotting of the CRN's.
Currently crnsimul supports plotting through the [Gnuplot](http://www.gnuplot.info) graphing utility and custom made OpenRTPlotter. 

#### 2.1 OpenRTPlotter
OpenRTPlotter is a custom made plotter for this project.
The reason for creating OpenRTPlotter was due to the fact that Gnuplot is unable to plot in real time which leads to some problems when simulating turing complete systems.
Besides that, OpenRTPlotter is generally faster, so it is recommended for all use cases.

If you wish to use the OpenRTPlotter to simulate the previous example, use the following command-line invocation: 
```c++
$ crnsimul -RTP examples/plus.crn
```
**-RTP** is a command line parameter which opens up the OpenRTPlotter. If crnsimul is correctly installed, the OpenRTPlotter will open up and begin to simulate the given CRN.

![Image of the OpenRTPlotter](https://raw.githubusercontent.com/RasmusRendal/crnsimul/milestone/1.0/documentation/AdditionExample.gif)
#### 2.2 GNU plotting 
Gnuplot is a graphing utility which also works in the plotting of CRNs.
Gnuplot does not plot in real time and can lead to problems with CRNs that either takes a very long time to simulate, or never terminate.
For very small CRNs, Gnuplot is still a viable tool for plotting, but OpenRTPlotter is the recommended tool for plotting and simulating.

To use Gnuplot instead of the OpenRTPlotter, the following command line will be used instead:
```c++
$ crnsimul -P examples/plus.crn
```
![Image of the Gnuplot](https://puu.sh/FKhhd/ef382aa6ed.png)

#### 3. Printing to .csv files
It is also possible to print out the CRN data to a **".csv"** file for further data processing.
To print the **".csv"** file of our example **plus.crn** module. The following command line will be used:

```c++
$ crnsimul examples/plus.crn -O plus.csv
```
This will produce a comma-seperated values file which contains the concentrations at each step of the simulation.

### 4. Evaluators
The crnsimul program offers two types of evaluations, namely the Euler evaluator and the Markov evaluator.
These two types of evaluators have their own command line parameter to specify which evaluation will be used.
The previous examples in this manual were evaluated with the Euler evaluator, as it is the default evaluator if no evaluations are specified. 


#### 4.1 Euler evaluator
As mentioned earlier, the Euler evaluator is the default evaluator which will be used when no evaluators have been specified in the command line. 
In order to implicitly specify the Euler evaluator, the **"-e"** command line parameter should be used.  
```c++
$ crnsimul examples/plus.crn -e
```
When working with the Euler evaluator.
It is also possible to specify the step size using the **"-S"** command line parameter followed by a number representing the new step size.
Furthermore, this also applies for specifying the threshold with the **"-T"** parameter instead.

The default step value is `0.01`, and the default threshold is `0.00001`

As an example, by increasing the step size and threshhold, we will be given a less precise, but faster evaluation of the plus CRN as seen below.
```c++
$ crnsimul examples/plus.crn -S 0.8 -T 0.0001 -RTP
```
![Image of the lower precision](https://puu.sh/FKj7i/7d890f8696.png)

If a too low simulation is used, it might result in a float overflow. The simulator will crash, and instruct you to decrease the step size.

#### 4.2 Markov evaluator
In order to access the Markov evaluator, you must specify it by using the **"-m"** parameter.

```c++
$ crnsimul examples/plus.crn -m
```
It is also possible to specify the threshold of the Markov evaluator by using the **"-t"** parameter followed by a number representing the new threshold. 
The threshold specifies the iteration at which the Markov evaluator will terminate.
By default, it is set to `5.0`

Note that **"-t"** and **"-T"** from the Euler evaluator are different from eachother as they dont specify the same threshold.

randomwalk.crn, a CRN module in the examples folder, will be used in this example to showcase the Markov evaluator. The following command line will be used:

```c++
$ crnsimul examples/randomwalk.crn -m -t 3000 -RTP
```
![Image of the Markov evaluator](https://raw.githubusercontent.com/RasmusRendal/crnsimul/milestone/1.0/documentation/RandomWalkExample.gif)

### 5. Stdout
When using the OpenRTPlotter for real time plotting. It is also possible for the terminal to print out the output of the CRN using the **"--stdout"** command line. This can be seen below.

![Image of the stdout functionality](https://puu.sh/FKjuK/f8623ae265.jpg)

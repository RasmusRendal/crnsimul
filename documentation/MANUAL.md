# crnsimul - Manual
This manual will cover the necessary information and instructions to operate crnsimul, a program for simulation of chemical reaction networks (CRN). The crnsimul program is programmed in the C++ programming language, and generated using the tools [FLEX](https://github.com/westes/flex/) and [BISON](https://github.com/westes/flex/). 
The crnsimul program was, alongside [chemilang](https://github.com/RasmusRendal/chemilang), developed under the same project by a group of university students from AAU.
The crnsimul program contains features such as real time plotting of CRNs, which will be further elaborated on later in this manual.

#### Prerequisites
This manual covers information of how to operate of the program and does not go over the steps of setting up the crnsimul program. It is therefore highly recommended that you read the [README file](https://github.com/RasmusRendal/crnsimul/blob/milestone/1.0/README.md) attached in the crnsimul repository as it mentions certain dependencies that are necessary for crnsimul to work, as well as how to compile the project.

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
      - [4.1 Euler evaluator](#41-euler-evaluator)
      - [4.2 Markov evaluator](#42-markov-evaluator)
    - [5. Stdout](#5-stdout)
<br />

### 1. Writing simple chemical reaction networks
The crnsimul program works on filetypes of the **".crn"** extension, where the program will take in the file as a command line parameter for simulation. 
An easy way to create these **".crn"** files is to create a new file of the **".txt"** format and save it as an **".crn"** format containing the chemical reaction network.

**Note:**
The following command is necessary to have when running UNIX as it is a **shebang** used for specifying what program should be called to run the script. In our language it is solely seen as a comment, but will be included in future examples in order to attain for a good convention.
``` c++
#!/usr/bin/env -S crnsimul -P 
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
The main feature of the crnsimul program is the ability to have real time plotting of the CRN's. Currently crnsimul supports plotting through the [Gnuplot](http://www.gnuplot.info) graphing utility and custom made OpenRTPlotter. 

#### 2.1 OpenRTPlotter
OpenRTPlotter is a custom made plotter for this project. The reason for creating OpenRTPlotter was due to the fact that Gnuplot is unable to plot in real time which leads to some problems when simulating turing complete systems.

If you wish to use the OpenRTPlotter to simulate the previous example. Create a **".crn"** file out of it and have it saved inside the crnsimul repository. In this example, the following file is called **plus.crn** and located in the examples folder. Locate the repository inside your terminal and write the following command line:
```c++
./bin/crnsimul -RTP examples/plus.crn
```
**-RTP** is a command line parameter which opens up the OpenRTPlotter. If crnsimul is correctly installed, the OpenRTPlotter will open up and begin to simulate the given CRN.

![Image of the OpenRTPlotter](https://puu.sh/FKh2r/d2064a145b.png)
#### 2.2 GNU plotting 
Gnuplot is a graphing utility which also works in the plotting of CRNs. Gnuplot does not plot in real time and can lead to problems with CRNs that either takes a very long time to simulate, or never ends. For very small CRNs, Gnuplot is still a viable tool for plotting, but OpenRTPlotter is still the recommended tool for plotting and simulating.

To use Gnuplot instead of the OpenRTPlotter, the following command line will be used instead:
```c++
./bin/crnsimul -P examples/plus.crn
```
![Image of the Gnuplot](https://puu.sh/FKhhd/ef382aa6ed.png)

#### 3. Printing to .csv files
It is also possible to print out the CRN data to a **".csv"** file for further data processing. To print the **".csv"** file of our example **plus.crn** module. The following command line will be used:

```c++
./bin/crnsimul examples/plus.crn -O [Name of file]
```
By calling our file **plusCsv**, a **".csv"** file of this name can be found in the crnsimul repository. 

### 4. Evaluators
The crnsimul program offers two types of evaluations, namely euler evaluator and Markov evaluator. These two types of evaluators have their own command line parameter to specify which evaluation will be used. The previous examples in this manual were evaluated with the euler evaluator, as it is the default evaluator if no evaluations are specified. 


#### 4.1 Euler evaluator
As mentioned earlier, euler evaluator is the default evaluator which will be used when no evaluators have been specified in the command line. 
In order to implicitly specify the euler evaluator, the **"-e"** command line parameter will be used.  
```c++
./bin/crnsimul examples/plus.crn -e
```
When working with the euler evaluator. It is also possible to specify the step size using the **"-S"** command line parameter followed by a number representing the new step size. Furthermore, this also applies for specifying the threshold with the **"-T"** parameter instead.

These are the predefined values of step size and threshold:
```c++
double estep = 0.01;
double ethreshold = 0.00001;
```

As an example, by increasing the step size and threshhold, we will be given a less precise evaluation of the plus CRN as seen below.
```c++
./bin/crnsimul examples/plus.crn -S 0.8 -T 0.0001 -RTP
```
![Image of the lower precision](https://puu.sh/FKj7i/7d890f8696.png)

#### 4.2 Markov evaluator
In order to access the Markov evaluator, you must implicitly specify it by using the **"-m"** parameter.

```c++
./bin/crnsimul examples/plus.crn -m
```
It is also possible to specify the threshold of the Markov evaluator by using the **"-t"** parameter followed by a number representing the new threshold. 
This is the predefined value of the Markov evaluator threshold:
```c++
double timeThreshold = 5.0;
```

Note that **"-t"** and **"-T"** from the euler evaluator are different from eachother as they dont specify the same threshold.

randomwal.crn, a CRN module in the examples folder, will be used in this example to showcase the Markov evaluator. The following command line will be used:

```c++
./bin/crnsimul examples/randomwalk.crn -m -t 2 -RTP
```
![Image of the Markov evaluator](https://puu.sh/FKjlj/778df5d515.png)

### 5. Stdout
When using the OpenRTPlotter for real time plotting. It is also possible for the terminal to print out the output of the CRN using the **"--stdout"** command line. This can be seen below.

![Image of the stdout functionality](https://puu.sh/FKjuK/f8623ae265.jpg)

# Chemilang codebase
This is the codebase for chemilang. Have fun hacking
## Building
The project uses cmake. To compile a project using cmake, the following commands should be used:
```sh
cmake .
make
```
Besides that, there currently is also one submodule. Run
```
git submodule update --init --recursive
```
to download it
Afterwards, you should be able to find the executable in the ./bin folder
## Dependencies
The dependencies can be found in the dockerfile. For Arch Linux, they can be installed by running
```
sudo pacman -S clang flex bison boost gnuplot gtest
```
## Better auto-complete with language servers
Language servers require a `compile_commands.json` file to be present to enable better autocompletion support. To generate it when running cmake, modify your cmake command to be
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```
For convenience, you can add `alias cmake="cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1"` to your `.bashrc`.
## Formatting
Github has a CI step implemented that requires that all code is compliant with the `.clang-format` file in the root of the project directory. You can use the `format.sh` file to format the code automatically, or use a tool that implements clang-format on save for instance. For vim I can recommend [neoformat](https://github.com/sbdchd/neoformat).

## Documentation
As an additional feature doxygen commentens have been placed in the code. Doxygen is a documentation generator that can output documentation to either html or latex format. Doxygen also generates class diagrams, inheritance diagrams and collaboration diagrams. In order for this to work graphviz should be installed, since it is the package used for generating these.
For Arch Linux, they can be installed by running
```
sudo pacman -S doxygen graphviz
```
If you would like to generate documentation, but do not need/want the output from graphviz you could set the `HAVE_DOT` = NO in the Doxyfile.
To use doxygen you would use the following command
```
doxygen Doxyfile
```
This will generate html and latex based on the configuration values in the Doxyfile.
It is recommended to use the html documentation, since it looks prettier and has better hyperlinking. 

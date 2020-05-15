# crnsimul
crnsimul is an application that simulates Chemical Reaction Networks.
## Trying it out
If you have installed the program, you can just execute them using `./examples/multiply.crn`, and they will run. If not, you need to specify the interpreter you are using explicitly, so `./bin/crnsimul -P examples/multiply.crn`.

See the examples directory for example reaction networks.

Also, see the [manual](https://github.com/RasmusRendal/crnsimul/blob/milestone/1.0/MANUAL.md).
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

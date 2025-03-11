# Create a Docker container for AlmaLinux 9   
Instructions on how to create a Docker container for AlmaLinux 9 on Ubuntu 24.04.02 LTS.  
Based on [SevenBridges](https://docs.sevenbridges.com/docs/install-docker-on-linux) and the [Manual](https://alicedoimo.github.io/docker-almalinux-guide/) by Alice Doimo.

## Install Docker  

Installing Docker requires `sudo` priviliges.

1. Ensure that the tool `cURL` is installed. If not, you can install it by:  
```
$ sudo apt-get update  
$ sudo apt-get install curl  
```
2. Get the latest Docker package by:  
```
$ sudo curl -fsSL https://get.docker.com/ | sh  
```  
You can check whether Docker is installed by:  
```
$ docker --version  
```  
which should result in:  
```
Docker version XX.X.X, build XXXXXXX
```  
3. Finally, add your account to the `docker` group in order to run Docker without `sudo` priviliges:  
```
$ sudo usermod -aG docker <your_username>
```  
  
Based on [SevenBridges](https://docs.sevenbridges.com/docs/install-docker-on-linux).

## Load the AlmaLinux 9 container in Docker  

1. Dowload the AlmaLinux 9 image:  
```
$ docker pull almalinux:9
```  
This image should now be visible in the Docker images:  
```
$ docker images
```  
2. Create a container from the AlmaLinux 9 image:  
```
$ docker run --name my_container_name -it almalinux:9 /bin/bash
```  
The `run` command creates the container.  
The `--name` tag assigns a name `"my_container_name"` to the container.  
The `it` tag creates an interactive session while `/bin/bash` starts a bash terminal in the container.  
The container should now be visible in the Docker containers:  
```
$ docker ps -a
```  
3. Check the AlmaLinux 9 installation:  
Inside the bash terminal of the container use:  
```
$ cat /etc/os-release
```  
to list the available containers, which should return:  
```
NAME="AlmaLinux"    
VERSION="9.5 (Teal Serval)"  
...
```  
  
Based on the [Manual](https://alicedoimo.github.io/docker-almalinux-guide/) by Alice Doimo.

## Use the AlmaLinux 9 container in Docker  

To start an existing container:  
```
$ docker start -ai my_container_name
```  
where the `-ai` flag ensures the proper stdout and stdin.  
This command will open the bash terminal inside the Docker container. 

You can exit and stop the container using the `exit` command or by pressing `Ctrl+D`.  
If you would like to close the terminal but leave the container running, you can detach the screen by pressing `Ctrl+P` and `Ctrl+Q`.  
  
For further functionality, consult the Docker [documentation](https://docs.docker.com/reference/).

## Mount a volume to a container in Docker

In order to avoid installing editors in the container, files can be shared between the host machine and the target container. Files can then be edited on the host machine and executed in the container.  
Volumes cannot be mounted to existing containers, so this has to be done when the container is created.  
To mount a volume the flag `-v` is added to the `run` command:  
```
$ docker run -it --name "my_container_name" -v "full_path_on_host_machine:target_folder_in_container" almalinux:9
```  
Now files inside the "full_path_on_host_machine" directory are visible in the "target_folder_in_container". You can use your favourite editor on your host machine to edit files in the container.  
  
Based on [Diona Rodrigues](https://dionarodrigues.dev/blog/how-to-use-docker-images-containers-volumes-and-bind-mounts).

## Install Julia in AlmaLinux 9

To install `Julia` on Linux use the command:  

```
$ curl -fsSL https://install.julialang.org | sh -s -- --yes
```  
where the `--yes` flag installs `Julia` in non-interactive mode.   
As indicated by the installer, you might have to set the `Julia` PATH in the correct root files to run `Julia` from any location:  
```
$ . /root/.bashrc
```   
  
To start an interactive session in the command-line REPL, type:
```
$ julia
```  
To run a `Julia` script, type:  
```
$ julia my_script.jl
```  
  
To install packages in `Julia` first open an interactive REPL session.  
Inside the REPL, press the `]` key, which will open the `Pkg` package manager.  
To install a package, use:  
```
pkg> add my_package 
```  
To exit the `Pkg` environment, press `Ctrl+C`.  
To exit the REPL session entirely, press `Ctrl+D`.  
  
For further information, check out the `Julia` [documentation](https://docs.julialang.org/en/v1/)

## Compile C++ in AlmaLinux 9

If necessary first install the `sudo` command through:  
```
$ dnf install sudo
```  
Check if the C++ compiler `g++` is installed:   
```
$ g++ --version
```  
If the `g++` command is not found, procede to explicitly installing it:  
```
$ sudo dnf install g++
```  
and check the version again.  

With this compiler, we can now compile C++ scripts:  
```
$ g++ my_script.cpp -o my_programme
```  
This line of code compiles the C++ script my_script.cpp and creates an executable programme named my_programme. The programme can then be executed as:  
```
$ ./my_programme
```  
  
Based on [OrcaCore](https://orcacore.com/install-gcc-compiler-almalinux-9/) and [java frm](https://www.youtube.com/watch?v=m05leBnnVZU).
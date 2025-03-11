---  
# Create a Docker container for AlmaLinux 9  
---  
Instructions on how to create a Docker container for AlmaLinux 9 on Ubuntu 24.04.02 LTS.  
Based on [SevenBridges](https://docs.sevenbridges.com/docs/install-docker-on-linux) and the [Manual](https://alicedoimo.github.io/docker-almalinux-guide/) by Alice Doimo.

## Install Docker  
---

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

## Load the AlmaLinux 9 container in Docker
---

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
$ docker run --name "my_container_name" -it almalinux:9 /bin/bash
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

## Use the AlmaLinux 9 container in Docker
---
To start an existing container:  
```
$ docker start -ai "my_container_name"
```  
where the `-ai` flag ensures the proper stdout and stdin.  
This command will open the bash terminal inside the Docker container. 

You can exit and stop the container using the `exit` command or by pressing `Ctrl+D`.  
If you would like to close the terminal but leave the container running, you can detach the screen by pressing `Ctrl+P` and `Ctrl+Q`.  
  
For further functionality, consult the Docker [documentation](https://docs.docker.com/reference/).


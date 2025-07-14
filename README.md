# CICADA input generation
Repository to make CICADA input generation fully open source, using containers

## Prerequisites
This setup requires either **Docker** or **Podman**. For a detailed introduction, see the [CMS Open Data Docker Tutorial](https://cms-opendata-workshop.github.io/workshop2023-lesson-docker/03-docker-for-cms-opendata/index.html).

This workflow uses the `cmsopendata/cmssw_10_6_30-slc7_amd64_gcc700` container image, as documented [here](https://opendata.cern.ch/docs/cms-guide-docker), for working with Run 2 data.

## Setup Instructions

> [!NOTE]
> This workflow has been tested on both a personal mac and the university linux cluster (without root privileges) 

Follow the steps below to set up the environment and generate the inputs.

### 1. Start the Container

First, launch the container using the appropriate command for the system. This will mount a local directory into the container where all our work will be stored.

* **On a Linux System (e.g., University Cluster):**
    * **Important:** Use a local storage path (like `/scratch/` or `/tmp/`), not a network path like `/nfs/` or `/afs/`, to avoid issues. Use `chmod -R 777` to set the permissions for `/path/to/local/storage` on the host and for `/code/CMSSW_10_6_30` from inside the container.
    ```bash
    podman run -it --name cicada_dev --net=host --userns=keep-id --env="DISPLAY" -v $HOME/.Xauthority:/home/cmsusr/.Xauthority:rw -v /path/to/local/storage:/code:Z cmsopendata/cmssw_10_6_30-slc7_amd64_gcc700 /bin/bash
    ```

* **On macOS:**
    ```bash
    docker run -it --name cicada_dev -v ${HOME}/cicada_work:/code cmsopendata/cmssw_10_6_30-slc7_amd64_gcc700 /bin/bash
    ```
> **Note:** The first time we run this, it will download the CMSSW image (~7 GB), which may take a while, let's admire the scenery while it's done `:P`

### 2. Set up the CMSSW Environment

Once inside the container, navigate to the `src` directory and set up the CMSSW environment.

```bash
cd /code/CMSSW_10_6_30/src
cmsenv
```
**Validate the Environment:** Once the `cmsenv` command runs successfully, we can test if everything works by following [this part](https://cms-opendata-workshop.github.io/workshop2023-lesson-docker/04-validation/index.html) of the opendata tutorial.

### 3. Clone This Repository

To get the needed files do `git clone git@github.com:abhi-mal/cicada_inputs.git`

### 4. Place Input Files

Create a directory for the input .root files and place them there.

```
mkdir -p /code/input_files
# Now, copy our input .root files into this directory
```

### 5. Generate CICADA Inputs

Get the inputs using the generate_cicada_inputs.py script which can be run as usual using `python generate_cicada_inputs.py`
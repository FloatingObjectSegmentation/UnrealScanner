## PointCloudia


PointCloudia is a tool for selecting and exporting a subset of points within a point cloud, possibly for the purpose of labeling the point cloud.

For now, the program is primarily meant to be used with LiDAR point clouds.

### Dependencies

- Unreal Engine (You need to have plugins in your 4_19/Engine/Plugins folder):
    - Version 4.19
    - [UE4.19 PointCloudPlugin](https://forums.unrealengine.com/community/community-content-tools-and-tutorials/1430363-point-cloud-plugin)
    - [UE4.19 Rama's VictoryPlugin](https://forums.unrealengine.com/development-discussion/blueprint-visual-scripting/4014-39-rama-s-extra-blueprint-nodes-for-you-as-a-plugin-no-c-required?3851-(39)-Rama-s-Extra-Blueprint-Nodes-for-You-as-a-Plugin-No-C-Required!=)
- las2txt
    - Install LASTools directly from their github repository. Use the las2txt tools they have.
- [lidar-orto-photo project by Slemenik](https://github.com/slemenik/lidar-orto-photo)

### Install

- Generate the source files by right clicking on the ```.uproject``` file and clicking Generate Source Files.
- Open the ```.sln``` in Visual Studio 2017

### Prepare

1. Prepare your LAS point cloud
    - Do this step only if you are using ARSO data
        - Use ```lidar-orto-photo``` to enrich your ARSO point cloud with RGB data.
    - Use ```las2txt``` to transform your .LAS or .LAZ point cloud into the correct format.
        - ```las2txt -i yourpointcloud.laz -o cloud.txt -parse xyzRGB```
        - ```las2txt -i yourpointcloud.laz -o cloudclass.txt -parse xyrRGc```
2. Configure ```PointCloudRenderingComponent.h``` to use the above files for the main file and the file with classification data respectively. 

### Usage

- **Right mouse click**: Spawns a new selection box.
- **Left mouse click**: If a selection box is spawned, then this will finish the selection and export the file.
- **WASDQE**: move your pawn around.
- **UIOJKL**: move the selection box around.
- **1**: Switch to translation mode.
- **2**: Switch to rotation mode.
- **3**: Switch to scaling mode.


### Programming trace
Idea and program, dependency trace is on this [link](https://docs.google.com/document/d/1yHzkerjFujqTkWjpkrrbrEc0v5QezEtqUxGGcvH7Hhw/edit?usp=sharing)
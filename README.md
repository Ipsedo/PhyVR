# PhyVR NDK

## Mandatory

In `PhyVR/libs/cardboard/sdk/build/gradle` change the line :

```groovy
        main.proto.srcDirs = ["${project.rootDir}/proto"]
```

with

```groovy
        main.proto.srcDirs = ["../proto"]
```

or Proto will not build correctly
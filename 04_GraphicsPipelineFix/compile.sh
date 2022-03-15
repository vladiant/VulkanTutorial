#!/bin/bash

glslangValidator -V100 shaders/simple_shader.frag -o shaders/simple_shader.frag.spv
glslangValidator -V100 shaders/simple_shader.vert -o shaders/simple_shader.vert.spv

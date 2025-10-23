#!/bin/bash
echo "Creating directory structure for lab work..."

# Temp dir
mkdir -p tasks_tmp
echo "txt1" > ./tasks_tmp/txt1.txt
echo "txt2" > ./tasks_tmp/txt2.txt
echo "txt3" > ./tasks_tmp/txt3.txt
echo "hidden1" > ./tasks_tmp/hidden1.txt
echo "system1" > ./tasks_tmp/system1.txt
touch ./tasks_tmp/bmp1.bmp
touch ./tasks_tmp/bmp2.bmp
touch ./tasks_tmp/exe1.exe
touch ./tasks_tmp/exe2.exe

mv ./tasks_tmp/hidden1.txt ./tasks_tmp/.hidden1.txt

# Task 1a
mkdir -p task_01a

# Task 1b
mkdir -p task_01b
mkdir -p task_01b/subdir
mkdir -p task_01b/subdir/empty_subdir
touch task_01b/subdir/txt1.txt
touch task_01b/subdir/txt2.txt

mkdir -p task_01b/.hidden_dir
mkdir -p task_01b/.hidden_dir/empty_subdir
mkdir -p task_01b/.hidden_dir/subdir
touch task_01b/.hidden_dir/subdir/txt1.txt
touch task_01b/.hidden_dir/subdir/txt2.txt
touch task_01b/.hidden_dir/txt1.txt
touch task_01b/.hidden_dir/system1.txt

touch task_01b/readonly.txt
chmod 444 task_01b/readonly.txt
touch task_01b/nonarchieve.txt
touch task_01b/nonarchieve-readonly.txt
chmod 444 task_01b/nonarchieve-readonly.txt
touch task_01b/system-nonarchieve-readonly.txt
chmod 444 task_01b/system-nonarchieve-readonly.txt
touch task_01b/system-readonly.txt
chmod 444 task_01b/system-readonly.txt

# Task 1c
mkdir -p task_01c_src
cp ./tasks_tmp/txt1.txt ./task_01c_src/txt1.txt
cp ./tasks_tmp/txt1.txt ./task_01c_src/txt2.txt
cp ./tasks_tmp/txt1.txt ./task_01c_src/txt3.txt
cp ./tasks_tmp/txt1.txt ./task_01c_src/txt4.txt
cp ./tasks_tmp/txt1.txt ./task_01c_src/txt5.txt
mkdir -p task_01c_dst

# Task 1d
mkdir -p task_01d_1
mkdir -p task_01d_2
cp ./tasks_tmp/txt1.txt ./task_01d_1/
cp ./tasks_tmp/txt2.txt ./task_01d_1/
cp ./tasks_tmp/txt3.txt ./task_01d_1/
cp ./tasks_tmp/txt1.txt ./task_01d_2/
cp ./tasks_tmp/txt2.txt ./task_01d_2/
cp ./tasks_tmp/txt3.txt ./task_01d_2/

# Task 1e
mkdir -p task_01e
cp ./tasks_tmp/txt1.txt ./task_01e/txt1.txt
cp ./tasks_tmp/txt1.txt ./task_01e/txt2.txt
cp ./tasks_tmp/txt1.txt ./task_01e/txt3.txt
cp ./tasks_tmp/txt1.txt ./task_01e/txt4.txt
cp ./tasks_tmp/txt1.txt ./task_01e/txt5.txt

# Task 1f
mkdir -p task_01f
echo "Start" > ./task_01f/txt1.txt
echo "Hello World" >> ./task_01f/txt1.txt
echo "End" >> ./task_01f/txt1.txt
echo "Start" > ./task_01f/txt2.txt
echo "Hello, World!" >> ./task_01f/txt2.txt
echo "End" >> ./task_01f/txt2.txt

# Task 1g
mkdir -p task_01g
mkdir -p task_01g/subdir
mkdir -p task_01g/subdir/empty_subdir
touch task_01g/subdir/txt1.txt
touch task_01g/subdir/txt2.txt

mkdir -p task_01g/.hidden_dir
mkdir -p task_01g/.hidden_dir/empty_subdir
mkdir -p task_01g/.hidden_dir/subdir
touch task_01g/.hidden_dir/subdir/txt1.txt
touch task_01g/.hidden_dir/subdir/txt2.txt
touch task_01g/.hidden_dir/txt1.txt
touch task_01g/.hidden_dir/system1.txt

touch task_01g/readonly.txt
chmod 444 task_01g/readonly.txt
touch task_01g/nonarchieve.txt
touch task_01g/nonarchieve-readonly.txt
chmod 444 task_01g/nonarchieve-readonly.txt
touch task_01g/system-nonarchieve-readonly.txt
chmod 444 task_01g/system-nonarchieve-readonly.txt
touch task_01g/system-readonly.txt
chmod 444 task_01g/system-readonly.txt
touch task_01g/system.txt
touch task_01g/system-nonarchieve.txt

# Task 1h
mkdir -p task_01h

# Task 1i 
mkdir -p task_01i
mkdir -p task_01i/Russian
mkdir -p task_01i/English

# Task 1j
mkdir -p task_01j

# Task 1k
mkdir -p task_01k
mkdir -p task_01k/subdir
mkdir -p task_01k/empty_subdir

echo "Structure created!"

find . -type d | sort

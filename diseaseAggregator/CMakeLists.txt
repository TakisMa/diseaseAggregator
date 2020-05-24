cmake_minimum_required(VERSION 3.16)
project(diseaseAggregator)

set(CMAKE_CXX_STANDARD 11)

add_executable(diseaseAggregator main.cpp Functions.cpp Record.cpp RecordIDManagement.cpp Hashtable.cpp Bucket.cpp BucketList.cpp ListNode.cpp Tree.cpp TreeNode.cpp Heap.cpp HeapNode.cpp Commands.cpp FileManagement.cpp SignalHandling.cpp)
add_executable(worker Worker.cpp Functions.cpp Record.cpp RecordIDManagement.cpp Hashtable.cpp Bucket.cpp BucketList.cpp ListNode.cpp Tree.cpp TreeNode.cpp Heap.cpp HeapNode.cpp Commands.cpp SignalHandling.cpp)
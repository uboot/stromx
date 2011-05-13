project(streamtest)

include_directories(../..)
add_executable(streamtest
    main.cpp
    DataContainerTest.cpp
    Id2DataPairTest.cpp
    OperatorWrapperTest.cpp
    TestOperator.cpp
   )

target_link_libraries(streamtest stream cppunit dl)

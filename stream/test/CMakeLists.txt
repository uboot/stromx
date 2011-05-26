project(streamtest)

include_directories(..)

add_executable(streamtest
    main.cpp
    ../DataContainer.cpp
    DataContainerTest.cpp
    ../Id2DataMap.cpp
    ../Id2DataMapper.cpp
    ../Id2DataComposite.cpp
    Id2DataCompositeTest.cpp
    ../Id2DataPair.cpp
    Id2DataPairTest.cpp
    ../OperatorWrapper.cpp
    ../Operator.cpp
    ../None.cpp
    TestOperator.cpp
    OperatorWrapperTest.cpp
    ../OutputNode.cpp
    OutputNodeTest.cpp
    ../InputNode.cpp
    InputNodeTest.cpp   
    ../OperatorNode.cpp
    ../Thread.cpp
    ThreadTest.cpp
)

target_link_libraries(streamtest boost_thread cppunit dl)


project(streamtest)

include_directories(..)
include_directories(${CPPUNIT_INCLUDE_DIR})

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
    ../Operator.cpp
    ../None.cpp
    TestOperator.cpp
    ../OperatorWrapper.cpp
    OperatorWrapperTest.cpp
    ../OutputNode.cpp
    OutputNodeTest.cpp
    ../InputNode.cpp
    InputNodeTest.cpp   
    ../OperatorNode.cpp
    ../Thread.cpp
    ThreadTest.cpp
    ../Network.cpp
    NetworkTest.cpp
)

target_link_libraries(streamtest ${Boost_LIBRARIES} ${CPPUNIT_LIBRARY} dl)


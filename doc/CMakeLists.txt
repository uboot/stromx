if(DOXYGEN_FOUND AND DOC_INSTALL_DIR)
    install(DIRECTORY "${PROJECT_BINARY_DIR}/doc/html"
        DESTINATION "${DOC_INSTALL_DIR}/stromx"
    )
endif()

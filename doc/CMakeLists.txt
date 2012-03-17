if(DOXYGEN_FOUND AND DOC_INSTALL_DIR)
    install(DIRECTORY "${PROJECT_BINARY_DIR}/doc/html"
        DESTINATION share/doc/packages/stromx
    )
endif()

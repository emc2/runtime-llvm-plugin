# Assume all includes are in a directory named after the project.  If
# this changes, you'll need to add your include directories here.
#
# TASK: Check to make sure the include directories are correct.

set(INCLUDE_DIRS ${PROJECT_NAME})

# Install the include files

install(DIRECTORY ${INCLUDE_DIRS} DESTINATION include COMPONENT headers)

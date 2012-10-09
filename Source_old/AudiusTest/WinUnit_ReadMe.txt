* WinUnit *

This project uses WinUnit for running unit tests. Unit tests are run automatically when compiling
the release version of the solution. For performance reasons they are not run during debug builds.

To debug tests, ensure that AudiusTest is set as Startup Project and also enter the following in
the project properties under Configuration Properties->Debugging:

          Command: $(SolutionDir)Tools\WinUnit\WinUnit.exe
Command Arguments: "$(TargetPath)"
Working Directory: $(TargetDir)


More information about WinUnit can be found here:
http://msdn.microsoft.com/en-us/magazine/cc136757.aspx


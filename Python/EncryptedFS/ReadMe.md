#Encrypted Filesystem

This project implements a device driver that works as a passthrough filesystem which decrypts the encrypted file stored on disk and returns it to the caller of a given function.

##Files Needed

To run properly, you'll need fuse.py and encfsStarterCode.py and run using Python 2.7.

##Compiling

```
python encfsStarterCode.py test/ mountpoint
```
You can either create a test file in your test directory or you can make one by running a command in another terminal.

##Testing

Test the filesystem using a separate terminal while the initial program runs.
Make sure you're in the same directory as the fuse.py and encfsStarterCode.py.
After each test, make sure to open the file from the terminal using the following:
  ```
  cat mountpoint/fileName
  ```
Make sure that what you see is what you expect; i.e. the last thing you wrote to the file system.
Testing writing to a file:
  In a separate terminal from the one running the encfsStarterCode.py, run the following:
  ```
  echo hello > mountpoint/hello
  ```
Testing overwriting the previous statement:
  In the same terminal you just ran the previous code in, run the following:
  ```
  echo hi > mountpoint/hello
  ```
Testing overwriting the previous statement with a longer word:
  In the same terminal you just ran the previous code in, run the following:
  ```
  echo "How is your day going" > mountpoint/hello
  ```

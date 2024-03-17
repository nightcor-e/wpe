<div align="center">
  <img width=200 height=200 src="assets/davinci.png" />
  <br />
  Wallpaper Engine made with SDL2
  <hr />
</div>
<p>To run this software, you need the DLLs for FFmpeg and SDL2. Make sure to have these DLLs available in the system before executing the program.</p>

<h2>Execution</h2>

<p>When running the program from the console, it is necessary to provide a specific argument to indicate the path of the file that will be used as wallpaper.</p>

<pre>./WPE.exe file_path</pre>

<p>Replace <code>file_path</code> with the full path to the file you want to use as wallpaper.</p>

<h2>Required DLLs</h2>

<ul>
  <li><b>SDL2.dll</b>: A cross-platform development library providing low-level access to audio, keyboard, mouse, and display functions.</li>
  <li><b>avcodec-xx.dll</b> (part of FFmpeg): Handles audio and video encoding and decoding.</li>
  <li><b>avformat-xx.dll</b> (part of FFmpeg): Deals with multimedia container formats.</li>
  <li><b>avutil-xx.dll</b> (part of FFmpeg): Provides various utility functions used by other components of the library, including common multimedia processing tasks.</li>
</ul>
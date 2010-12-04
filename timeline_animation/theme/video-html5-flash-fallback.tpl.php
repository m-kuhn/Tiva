<?php
// $Id$

/*
 * Output as html5 video by default with flash fallback.
 *
 * Taken from
 * http://camendesign.com/code/
 *
 */
?>
<!-- first try HTML5 playback: if serving as XML, expand `controls` to `controls="controls"` and autoplay likewise       -->
<!-- warning: playback does not work on iPad/iPhone if you include the poster attribute! fixed in iOS4.0                 -->
<video width="<?php print $video->url['width']; ?>" height="<?php print $video->url['height']; ?>" controls>
	<!-- MP4 must be first for iPad! -->
	<source src="<?php print $video->url['video/mp4']; ?>" type="video/mp4" /><!-- WebKit video    -->
	<source src="<?php print $video->url['video/ogg']; ?>" type="video/ogg" /><!-- Firefox / Opera -->
	<!-- fallback to Flash: -->
	<object width="<?php print $video->url['width']; ?>" height="<?php print $video->url['height']; ?>" type="application/x-shockwave-flash" data="<?php print $video->url['flash']; ?>">
		<!-- Firefox uses the `data` attribute above, IE/Safari uses the param below -->
		<param name="movie" value="<?php print $video->url; ?>" />
		<param name="flashvars" value="controlbar=over&amp;image=<?php print $video->poster; ?>&amp;file=<?php print $video->url['video/mp4']; ?>" />
		<!-- fallback image. note the title field below, put the title of the video there -->
		<img src="<?php print $video->poster; ?>" width="<?php print $video->url['width']; ?>" height="<?php print $video->url['height']; ?>" alt="<?php print $video->name; ?>"
		     title="No video playback capabilities, please download the video below" />
	</object>
</video>
<!-- you *must* offer a download link as they may be able to play the file locally. customise this bit all you want -->
<p>	<strong>Download Video:</strong>
	Closed Format:	<a href="<?php print $video->url['video/mp4']; ?>">"MP4"</a>
	Open Format:	<a href="<?php print $video->url['video/ogg']; ?>">"Ogg"</a>
</p>
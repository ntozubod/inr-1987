# Organization of *ginr* repo branches

The branch history of INR has been a little confusing.
The several attempts to sort this out have all been incomplete and
added to the confusion.
In order to move toward a simpler structure with one *main* branch and
one *stable* branch, the previous candidate branches need to be named in
a consistent manner.

The former *remaster* branch has been replaced by *main2021*.
The former *stable* branch now will track the updates to *main2021*.
It will represent a 'stable' commit in the the *main2021* branch with
*main2021* sometimes leading to allow for bug fixes.
The *main2021* branch will correspond to INR version 2.0.x.

For completeness, *main2018*, *main2010*, and *main1988* branches have also
been created to facilitate access to previous notions of what a 'main'
branch should be.

Its commit history of *main2022* and *main2021* traces back through the
*redux* branch to the *main2010* branch, re-applying all of the changes
made by the *main2018* branch without the agressive reformatting.

# YASS (Yet Another Student Submission)

WIP: This repo stores the code for a new substrate with which students
can view their grades. It hopes to improve upon that which is, at the time
of this writing, currently handled by github README's
and pull requests by providing autograder test details and code review
comments all in one single view akin to that of
CS198's [paperless](https://paperless.stanford.edu) and CS107's gradebook
## Setup
1. npm install to install dependencies using the package.json config file
2. `node_modules/.bin/rollup -c rollup.config.mjs` to update any changes made
codemirrors `editor.mts`
3. Currently, I use the vs code extension [Live Server](https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer) to
serve index.html so that I am allowed to read JSON files from the
users device
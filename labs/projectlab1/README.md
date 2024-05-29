---
released: true
permalink: /labs/projectlab1/
title: 'Lab 8: Project Team Meeting 1'
readings: |
  + [Prelab](/labs/projectlab1/#prelab-preparation) to prep before lab.
  + Lab will be a project work session. Plan to attend same lab session as your teammates.
---
{% comment %}
__To prep for lab:__
- [ ] Remind students to attend lab together with their team
{% endcomment %}

### Goals

The final two weeks of lab sessions will be used as working meetings for the project teams. Plan to attend lab at same time as your teammates. 

Use the first week's meeting to work out your project plan and set the schedule. What cool aspect of computer systems will you explore?  What neat hardware do you want to involve? How will you coordinate with your team to execute on your plan? 

By the end of the lab session, you want to

- Settle on project plan
- Curate the hardware/materials required, identify where to source any needed components
- Establish practices for effective communication and coordination with your team
- Enumerate midpoint milestones to hit by next week's lab (aim to resolve unknowns/risks ASAP)
- Check-in with us to confirm your plans are on track for a great result

Refer to the [project writeup](/project/) for more information on the project itself.

## Prelab preparation
To prepare for lab, do the following:

Connect with your team to make a great shared start:
  - Establish norms for how you communicate and make decisions
  - Share with one another what you hope to learn, explore, or do in completing your project

### Setup project repository

You've been git-wrangling all quarter as part of your individual development process; the benefits of revision control become even more critical when coordinating work across multiple authors. 

This quick lab exercise is to get your team started on using git collaboratively. Each project has its own remote repository on GitHub; you should have received an email invitation to access it. This remote repository is the "drop point" where your team synchronizes the joint work. You also should have received an email invitation to access the project mirror repo, which contains a skeleton project structure (Makefile included!) that you can use a starting point for your project. 

The basic workflow goes like this. Each team member has a local copy of the repository on their laptop. Edit files in the local repository and commit changes. When your latest work has stabilized to a state that is ready to share, use `git push` to send your updates to the remote. Your teammate then uses `git pull` to fetch changes from the remote which are brought into their local repository. 

Each team member should change to their cs107e_home directory and make a local clone of the team's project repository:

```console
$ cd ~/cs107e_home
$ git clone git@github.com:cs107e/project-[YOUR-GITHUB_USERNAMES].git project
```

Have each team member do the following to confirm that your local project repository is correctly configured.
+ In the local project repository, edit the file `proposal.md` file and add your name to the list of team members.  Save the file.
+ Commit your change and push to Github.
+ Have your teammates pull your change.

During the pull/push operations, git will attempt to automatically merge changes made by different authors. However, if both you and a teammate have made overlapping edits, git will halt and report a "merge conflict". Git dumps both sets of changes into the conflicted file and fences the sections with marker symbols (`>>>`, `<<<`, `===`). You must manually resolve the sections in conflict. To do this, open the conflicted file, look for the marker symbols, edit the contents to select one/other/both changes as you see fit and remove the symbols. Save the file and commit it to complete the merge.

### Develop project plan and review with us

Brainstorm project options and refine those ideas into a specific project.  Make a list of parts you will need and figure out where you can get them, paying particular attention to shipping lead times. Breakdown the work into task areas and assign responsibilities to each team member. Draft a schedule of milestones for the next two weeks.

Grab an instructor or TA at some point during the lab and give a pitch for your project and to get our feedback on ensuring the project direction, scope, and schedule are aligned for a great outcome. Need advice on hardware, code, resources, strategy? Please ask!

We want every team to leave lab with a solid plan and a draft of your [project proposal](/project/#proposal). Proposals are due to be submitted by __end of day Thursday__.

### Check in with TA

Before the end of the lab period, be sure to confirm the following items:

* Can each of you access the team repository?

* What is your plan for acquiring any additional materials you need in a timely fashion? 

* What are the milestones to achieve by next week's lab?


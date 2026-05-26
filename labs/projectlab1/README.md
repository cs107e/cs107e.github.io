---
released: true
permalink: /labs/projectlab1/
toc: true
title: 'Lab 8: Project Team Meeting 1'
readings: |
  + [Prelab](/labs/projectlab1/#prelab-preparation) to prep before lab.
  + Lab will be a project work session. Plan to attend same lab session as your partner.
---
{% comment %}
__To prep for lab:__
- [ ] Remind students to attend lab together with their team
{% endcomment %}

## Goals

This lab session and next are a working meeting for your project team. In first project lab meeting, your team will work out the project plan and draft your proposal to submit.

### Prelab preparation
To prepare for lab, do the following:

- Review [project writeup](/project/) for project logistics and schedule.
- Plan to attend same lab session as your partner.
- Connect with your partner to
  - Establish norms for how you communicate and make decisions
  - Share with one another what you hope to learn, explore, or do in completing your project

## Setup your project repo

You've been git-wrangling all quarter as part of your individual development process; the benefits of revision control become even more critical when coordinating work across multiple contributors.

This quick lab exercise is to get your team started on using git collaboratively. Each project has its own remote repository on GitHub; you should have received an email invitation to access it. This remote repository is the "drop point" where your team synchronizes the joint work. You also should have received an email invitation to access the project mirror repo, which contains a skeleton project structure that you can use a starting point for your project.

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

During the pull/push operations, git will attempt to automatically merge changes made by different authors. However, if both you and your partner have made overlapping edits, git will halt and report a "merge conflict". Git dumps both sets of changes into the conflicted file and fences the sections with marker symbols (`>>>`, `<<<`, `===`). You must manually resolve the sections in conflict. To do this, open the conflicted file, look for the marker symbols, edit the contents to select one/other/both changes as you see fit and remove the symbols. Save the file and commit it to complete the merge.

Ensure that all team members are able to access the project repo and pull and push changes. Ask for help resolving any glitches![^1]

## Develop your project plan

What aspect of computer systems will you explore?  What hardware do you want to try out? How will you coordinate with your team to execute on your plan?

Brainstorm project options and refine those ideas into a specific project.  Make a list of components/materials/tools you will need and figure out where you can get them, paying particular attention to shipping lead times. [^3] Breakdown the work into task areas and assign responsibilities to each team member.

Start sketching a schedule of milestones that will guide your work. You will want to focus on resolving unknowns or risks sooner rather than later. What are the midpoint milestones you want to hit by next lab?[^4]

Grab a staff member and give a pitch for your project. Get our feedback on how to align project direction, scope, and schedule for a great outcome. Need advice on hardware, code, resources, strategy? Please ask![^2]

## Draft your project proposal, submit
After checking in with us to confirm your plans are on track for a great result, work with your partner to draft your project proposal. See [project writeup](/project) for instructions on what it to be included in the proposal and how to submit. Proposals are submitted at the end of lab or by __end of day Thursday__ if you need a little more time.

## Check in with TA

Before leaving lab, check in with us on your project plans.

<div class=checkinsheet markdown="1">
# {{ page.title }}
<div class="underline-team"></div>
Circle lab attended:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  _Tuesday_  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  _Wednesday_
<BR>
<BR>
Be sure to hand in check-in sheet to us, thanks![^5]
</div>

[^1]: Confirm each of you can access the team repository.
[^2]: Have you reviewed your project plan with us?
[^3]: What is your plan for acquiring any additional materials you need in a timely fashion?
[^4]: What are the milestones you aim to achieve by next week's lab?
[^5]: Do you need followup assistance from us? How can we help?


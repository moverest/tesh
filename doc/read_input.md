# Input reader doc

## Usage

Used by main when input is user typed from stdin. So the file execution must
be handle differently.

User will type a line of command. It will most likely be syntactically correct,
but it may not be and the user must be properly adverted if so.

A line of command will be computed and executed whenever the user type `enter`.

## Sequence diagram
<div style="background-color:rgb(255,255,255); border-radius:2px; padding:4px; margin:4px;">
<!--![sequence diagram](./tesh.png)-->
<img alt="sequence diagram" src="./tesh.png"/>
</div>


## Prompt
The terminal prompt's form is : `USER@HOSTNAME:REPCOURANT$`, with
* USER the name of the current user
* HOSTNAME the hostname of the machine
* REPCOURANT the working directory

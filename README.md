BugBlast
========

BugBlast is a simplified version of Bomberman game. In BugBlast, the player has to exterminate different nasty bugs using bug 
sprayer. After kill all of bugs within a given maze, the player can advance to the next maze. There are two different bugs,
simple and complex. Simple bug just move in a random direction but Complex bug can smell the player and move toward the player.

Input
========
<table>
  <tr>
    <th>Action</th><th>Keys</th>
  </tr>
  <tr>
    <td>Left</td><td>A, Left Arrow</td>
  </tr>
  <tr>
    <td>Right</td><td>D, Right Arrow</td>
  </tr>
  <tr>
    <td>Up</td><td>W, Up Arrow</td>
  </tr>
  <tr>
    <td>Down</td><td>S, Down Arrow</td>
  </tr>
  <tr>
    <td>Place Sprayer</td><td>Space</td>
  </tr>
  <tr>
    <td>Quit</td><td>Q</td>
  </tr>
</table>

Level 
========

Each level of BugBlast has a dfferent maze and different game parameters. This parameters are stored in data files, so 
the player can create various mazes on you own. The parameters look like the following,<br>
<pre>

############### 
#     *@*     #
# ####  ####  #
# * s*  *c *  #
# *  *  *  *  # 
#    #####    # 
#             # 
#             # 
#             #
#             #  
# #    *    # # 
#*************# 
#   *  #  *   #  
#  *   e   *  # 
###############

#:  Permanent Brick
*:  Destroyable Brick
@:  Player's Location
s:  Simple Bug
c:  Complex Bug
e:  Exit

</pre>

Screenshot
========
![screenshot](https://cloud.githubusercontent.com/assets/5351012/5212965/b82b3406-75b8-11e4-9e97-538ae2147470.png)

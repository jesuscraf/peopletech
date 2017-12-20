////////////////////////////////////////////////////////////
//
// (c) Copyright  1991-2000 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4index.js
// Project:
// Author:           Meta Software M.S. , S.A
// Language:         Javasccript
// Operating System: ALL
// Design Document:  
//
/////////////////////////////////////////////////////////////



<!--

function Folder(level, folderDescription, hreference) //CONSTRUCTOR
{
  //CONSTANT DATA
  this.desc = folderDescription
  this.hreference = hreference
  this.id = -1
  this.navObj = 0
  this.iconImg = 0
  this.nodeImg = 0
  this.isLastNode = 0
  this.level = level
  
  //DYNAMIC DATA
  this.isOpen = true
  this.iconSrc = Var[50]
  this.children = new Array
  this.nChildren = 0

  //METHODS
  this.initialize = initFolder
  this.setState = setStateFolder
  this.addChild = addChild
  this.createIndex = createEntryIndex
  this.hide = hideFolder
  this.display = display
  this.renderOb = drawFolder
  this.totalHeight = totalHeight
  this.subEntries = folderSubEntries
  this.outputLink = outputFolderLink
  this.outputLink2 = outputFolderTextLink
}

function setStateFolder(isOpen)
{
  var subEntries
  var totalHeight
  var fIt = 0
  var i=0

  if (isOpen == this.isOpen) return

  if (Var[1] == 2) {
    totalHeight = 0

    for (i=0; i < this.nChildren; i++)
      totalHeight = totalHeight + this.children[i].navObj.clip.height
	  
    subEntries = this.subEntries()

    if (this.isOpen) totalHeight = (0-totalHeight)

    for (fIt = this.id + subEntries + 1; fIt < Var[0]; fIt++)
      indexOfEntries[fIt].navObj.moveBy(0, totalHeight)
  }

  this.isOpen = isOpen
  propagateChangesInState(this)
  return
}

function propagateChangesInState(folder)
{
  var i=0

  if (folder.isOpen) {
    if (folder.nodeImg) {
      if (folder.isLastNode) folder.nodeImg.src = (Var[56])
      else folder.nodeImg.src = (Var[54])
	 }

    //if ((Var[20]) && (DOFimages[folder.id])) folder.iconImg.src = (Var[62])
    //else folder.iconImg.src = (Var[50])
    folder.iconImg.src = (Var[50])

    for (i=0; i<folder.nChildren; i++)
      folder.children[i].display()
  }
  else {
    if (folder.nodeImg) {
      if (folder.isLastNode) folder.nodeImg.src = (Var[55])
      else folder.nodeImg.src = (Var[57])
	 }

    //if ((Var[20]) && (CFimages[folder.id])) folder.iconImg.src = (Var[63])
    //else folder.iconImg.src = (Var[51])
    folder.iconImg.src = (Var[51])

    for (i=0; i<folder.nChildren; i++)
      folder.children[i].hide()
  }
  return
}

function hideFolder()
{
  if (Var[1] == 1) {
    if (this.navObj.style.display == "none") return
    this.navObj.style.display = "none"
  }
  else {
    if (this.navObj.visibility == "hidden") return
    this.navObj.visibility = "hidden"
  }

  this.setState(0)
  return
}

function initFolder(level, lastNode, leftSide)
{
  var i=0
  var nc = this.nChildren
  var auxEv = ""
  var a = ""
  var img1 = Var[54]
  var img2 = Var[58]
  var LN = 0

  this.createIndex()

  // EMPLOY (+/-) SIGN ASSOCIATION
  if ((Var[1]>0) && (Var[16]>0)) {
    if (Var[16]>1) auxEv=auxEv+"<a href='javascript:clickOnSign("+this.id+")' onMouseover='javascript:clickOnSign("+this.id+")'; return true>"
	else auxEv = "<a href='javascript:clickOnSign("+this.id+")'; return true>"
	a = "</a>"
  }

  if (level>0) {
    if (lastNode) {   // LAST BROTHER IN THE CHILDREN ARRAY
	  img1 = Var[56]
	  img2 = Var[59]
	  LN = 1
    }
    this.renderOb(leftSide+auxEv+"<img name='nodeIcon"+this.id+"' src='"+img1+"' border=0>"+a)
    leftSide = leftSide+"<img src='"+img2+"' border=0>"
    this.isLastNode = LN
  }
  else
    this.renderOb("")

  if (nc > 0) {
    level = level + 1
    for (i=0 ; i < this.nChildren; i++) {
      if (i == this.nChildren-1) this.children[i].initialize(level, 1, leftSide)
      else this.children[i].initialize(level, 0, leftSide)
    }
  }
  return
}

function drawFolder(leftSide)
{

  if (Var[1] == 2) {
    if (!document.yPos) document.yPos=8
    document.write("<layer id='folder"+this.id+"' top="+document.yPos+" visibility=hidden>")
  }

  if ((this.id==0) && Var[21]) document.write("<table border=0 cellspacing=0 cellpadding=0><tr><td valign=middle nowrap><img src="+Var[53]+"><br></td></tr></table>")

  if (Var[1] == 1) document.write("<table id='folder"+this.id+"' style='position:block;' border=0 cellspacing=0 cellpadding=0><tr><td valign=middle nowrap>"+leftSide)
  else document.write("<table border=0 cellspacing=0 cellpadding=0><tr><td valign=middle nowrap>"+leftSide)
  
  this.outputLink() //CALL FOLDER ICON

//  if (this.id==0) document.write("<br>")

//  if ((Var[20]) && (DOFimages[this.id])) document.write("<img name='folderIcon"+this.id+"' src='"+Var[62]+"' border=0></a></td><td valign=middle nowrap>")
//  else document.write("<img name='folderIcon"+this.id+"' src='"+this.iconSrc+"' border=0></a></td><td valign=middle nowrap>")
	document.write("<img name='folderIcon"+this.id+"' src='"+this.iconSrc+"' border=0></a></td><td valign=middle nowrap>")

//  if (this.id==0) document.write("<br>")

  if ((Var[14]>0) && (this.hreference)) this.outputLink2()  //CALL TO ASSOCIATE LINK WITH FOLDER TEXT
  else document.write(Var[5]+this.desc)
  
  document.write("</td></table>")

  if (Var[1] == 2) document.write("</layer>")

  if (Var[1] == 1) {
    this.navObj = document.all["folder"+this.id]
    this.iconImg = document.all["folderIcon"+this.id]
    this.nodeImg = document.all["nodeIcon"+this.id]
  }
  else if (Var[1] == 2) {
    this.navObj = document.layers["folder"+this.id]
    this.iconImg = this.navObj.document.images["folderIcon"+this.id]
    this.nodeImg = this.navObj.document.images["nodeIcon"+this.id]
    document.yPos=document.yPos+this.navObj.clip.height
  }
  return
}

function outputFolderLink()
{
  if ((this.hreference) && (Var[17] % 2)) {
    if (Var[1]>0) {
	  if ((Var[17]==1) || (Var[17]==5)) document.write("<a href="+this.hreference+" onClick='javascript:clickOnFolder("+this.id+")' onMouseover='javascript:clickOnFolder("+this.id+")'; return true>")
	  else document.write("<a href="+this.hreference+" onClick='javascript:clickOnFolder("+this.id+")'; return true>")
	}
	else document.write("<a href="+this.hreference+">")
  }

  else {
    if ((Var[17]==2) || (Var[17]==6) || (Var[17]==1) || (Var[17]==5)) document.write("<a href='javascript:clickOnFolder("+this.id+")' onMouseover='javascript:clickOnFolder("+this.id+")'; return true>")
	else document.write("<a href='javascript:clickOnFolder("+this.id+")'; return true>")
  }
  return
}

function outputFolderTextLink()
{
  // USE OF TEXT LINKS WITH FOLDER
  if ((this.hreference) && (Var[14]>0)) {
    if (Var[1]>0) {
	  if ((Var[14]==2) || (Var[14]==4)) document.write(Var[5]+"<a href="+this.hreference+" onClick='javascript:clickOnFolder("+this.id+")' onMouseover='javascript:clickOnFolder("+this.id+")'"+this.hreference+"'; return true>"+this.desc+"</a>")
	  else document.write(Var[5]+"<a href="+this.hreference+" onClick='javascript:clickOnFolder("+this.id+")'; return true>"+this.desc+"</a>")
	}
    else document.write(Var[5]+"<a href="+this.hreference+">"+this.desc+"</a>")
  }
  return
}

function addChild(childNode)
{
  this.children[this.nChildren] = childNode
  this.nChildren++
  return childNode
}

function folderSubEntries()
{
  var i = 0
  var se = this.nChildren

  for (i=0; i < this.nChildren; i++)
    // IS A FOLDER
    if (this.children[i].children) se = (se + this.children[i].subEntries())

  return se
}

// DEFINITION OF CLASS Item (a document or link inside a Folder)
// *************************************************************

function Item(itemDescription, itemLink, target, linkData) //CONSTRUCTOR
{
  // CONSTANT DATA
  this.desc = itemDescription
  this.link = itemLink
  this.id = -1      //INITIALIZED IN initalize()
  this.navObj = 0   //INITIALIZED IN render()
  this.iconImg = 0  //INITIALIZED IN render()
  this.target = target
  this.data = linkData

  // METHODS
  this.initialize = initItem
  this.createIndex = createEntryIndex
  this.hide = hideItem
  this.display = display
  this.renderOb = drawItem
  this.totalHeight = totalHeight
}

function hideItem()
{
  if (Var[1] == 1) {
    if (this.navObj.style.display == "none") return
    this.navObj.style.display = "none"
  }
  else {
    if (this.navObj.visibility == "hidden") return
    this.navObj.visibility = "hidden"
  }
  return
}

function initItem(level, lastNode, leftSide)
{
  var img1 = Var[60]
  var img2 = Var[58]

  this.createIndex()

  if (level>0) {
    if (lastNode) {     //THE LAST BROTHER IN THE CHILDREN ARRAY
	   img1 = Var[61]
	   img2 = Var[59]
    }
    this.renderOb(leftSide+"<img src='"+img1+"' border=0>")
    leftSide = leftSide+"<img src='"+img2+"' border=0>"
  }
  else this.renderOb("")
  return
}

function drawItem(leftSide)
{
  if (Var[1] == 2) document.write("<layer id='item"+this.id+"' top="+document.yPos+" visibility=hidden><table border=0 cellspacing=0 cellpadding=0><tr><td valign=middle nowrap>"+leftSide)

  if (Var[1] == 1) document.write("<table id='item"+this.id+"' style='position:block;' border=0 cellspacing=0 cellpadding=0><tr><td valign=middle nowrap>"+leftSide)

  if ((Var[18]>0) && (this.link!="")) {
	if (Var[18]==2) {
      if ((this.target==0) || (this.target==3) || (this.target==6))
        document.write("<a href="+this.link+" onMouseover=parent.frames[Var[3]].location="+this.link+"; return true><img id='itemIcon"+this.id+"' ")
      else if ((this.target==1) || (this.target==4) || (this.target==3))
        document.write("<a href="+this.link+" onMouseover=window.open('"+this.data+"'); return true><img id='itemIcon"+this.id+"' ")
      else
        document.write("<a href="+this.link+" onMouseover=parent.top.location="+this.link+"; return true><img id='itemIcon"+this.id+"' ")
	}
    else document.write("<a href="+this.link+"><img id='itemIcon"+this.id+"' ")
  }
  else document.write("<img id='itemIcon"+this.id+"' ")

  //if ((Var[20]) && (DOFimages[this.id])) document.write("src='"+Var[62]+"' border=0>")
  //else document.write("src='"+Var[52]+"' border=0>")
  document.write("src='"+Var[52]+"' border=0>")

  if ((Var[18]>0) && (this.link!="")) document.write("</a></td><td valign=middle nowrap>")
  else document.write("</td><td valign=middle nowrap>")

  if ((Var[15]>0) && (this.link!="")) {
    if (Var[15]==2) {
      if ((this.target==0) || (this.target==3) || (this.target==6))
        document.write(Var[5]+"<a href="+this.link+" onMouseover=parent.frames[Var[3]].location="+this.link+"; return true>"+this.desc+"</a></table>")
      else if ((this.target==1) || (this.target==4) || (this.target==3))
        document.write(Var[5]+"<a href="+this.link+" onMouseover=window.open('"+this.data+"'); return true>"+this.desc+"</a></table>")
      else
        document.write(Var[5]+"<a href="+this.link+" onMouseover=parent.top.location="+this.link+"; return true>"+this.desc+"</a></table>")
	}
    else document.write(Var[5]+"<a href="+this.link+">"+this.desc+"</a></table>")
  }
  else document.write(Var[5]+this.desc+"</table>")

  if (Var[1] == 2) document.write("</layer>")

  if (Var[1] == 1) {
    this.navObj = document.all["item"+this.id]
    this.iconImg = document.all["itemIcon"+this.id]
  }
  else if (Var[1] == 2) {
      this.navObj = document.layers["item"+this.id]
      this.iconImg = this.navObj.document.images["itemIcon"+this.id]
      document.yPos=document.yPos+this.navObj.clip.height
  }
  return
}


// METHODS COMMON TO BOTH OBJECTS (pseudo-inheritance)
// ********************************************************

function display()
{
  if (Var[1] == 1) this.navObj.style.display = "block"
  else this.navObj.visibility = "show"
  return
}

function createEntryIndex()
{
  this.id = Var[0]
  indexOfEntries[Var[0]] = this
  Var[0]++
  return
}

// TOTAL HEIGHT OF SUBENTRIES OPEN
function totalHeight() //USED WITH [Var[1]] == 2
{
  var h = this.navObj.clip.height
  var i = 0

  // IS A FOLDER AND _IS_ OPEN
  if (this.isOpen)
    for (i=0 ; i < this.nChildren; i++)
      h = h + this.children[i].totalHeight()

  return h
}


// EVENTS
// *********************************************************

// USE OF CLICK METHOD ON SIGN LINKS
function clickOnSign(folderId)
{
  var clicked = indexOfEntries[folderId]
  var topBranch = this.indexOfEntries[this.Var[23]]
  var prevBranch = this.indexOfEntries[this.Var[22]]
  var i

  
  // COLLAPSE THE BRANCH WHEN ACCESSING A NEW BRANCH
  if (this.Var[2]!=0) {	
    if (this.Var[22]!=folderId) {
	  if ((this.Var[22]!=0) && (prevBranch.isOpen) && (clicked.level<=prevBranch.level)) clickOnNode(this.Var[22])
	  if (clicked.level==1) {
        if ((this.Var[23]!=folderId) && (this.Var[23]!=0) && (topBranch.isOpen)) {
		  for (i=0; i<topBranch.nChildren; i++)
		    if (topBranch.isOpen) clickOnNode(this.Var[23])
		}
		this.Var[23]=folderId
	  }
	  this.Var[22] = folderId
	}
  }
  clickOnNode(folderId)
  return
}


// USE OF CLICK METHOD ON FOLDER LINK
function clickOnFolder(folderId)
{
  var clicked = indexOfEntries[folderId]
  var topBranch = this.indexOfEntries[this.Var[23]]
  var prevBranch = this.indexOfEntries[this.Var[22]]
  var i

  if ((this.Var[17]>0) && (this.Var[17]<5)) {
  
	// COLLAPSE THE BRANCH WHEN ACCESSING A NEW BRANCH
	if (this.Var[2]!=0) {	
	  if (this.Var[22]!=folderId) {
	    if ((this.Var[22]!=0) && (prevBranch.isOpen) && (clicked.level<=prevBranch.level)) clickOnNode(this.Var[22])
	    if (clicked.level==1) {
          if ((this.Var[23]!=folderId) && (this.Var[23]!=0) && (topBranch.isOpen)) {
		    for (i=0; i<topBranch.nChildren; i++)
		      if (topBranch.isOpen) clickOnNode(this.Var[23])
		  }
		  this.Var[23]=folderId
	    }
	    this.Var[22] = folderId
	  }
	}
    clickOnNode(folderId)
  }

  else if (this.Var[17]>4)
    if (!clicked.isOpen) clickOnNode(folderId)

  return

}

function clickOnNode(folderId)
{
  var clickedFolder = 0
  var state = 0

  clickedFolder = indexOfEntries[folderId]
  state = clickedFolder.isOpen

  clickedFolder.setState(!state) //open<->close
  return
}

function refresh()
{
  if (!indexOfEntries[0].isOpen) clickOnNode(0)
  return
}

function initDoc()
{
  var i = 0

  if (document.all) Var[1] = 1   // IE4.x
  else if (document.layers) Var[1] = 2   // NS4.x
  else Var[1] = 0   // other

  aux0.initialize(0, 1, "")
  aux0.display()

  if (Var[1] > 0) {
    if (Var[1]==2) document.write("<layer top="+indexOfEntries[Var[0]-1].navObj.top+" visibility=hidden></layer>")

    clickOnFolder(0)   // CLOSE THE ENTIRE TREE
	
	if (Var[19]==1) clickOnNode(0)   // OPEN THE ROUTE FOLDER ==> LEVEL 1

	if ((Var[19] == 0) && (Var[2]==0))   // OPEN ENTIRE TREE-FOLDER
	  for (i=0; i<Var[0]; i++)
	    if (this.indexOfEntries[i].children) clickOnFolder(i)
  }
  return
}


// AUXILIARY FUNCTIONS FOR FOLDER-TREE BACKWARD COMPATIBILITY
// **********************************************************

function getFolder(level, target, description, hreference)
{
  var fullLink = ""

  if ((hreference != "") && (target != (-1))) {
    fullLink = "'"+hreference+"' target=\""+Var[4]+"\""
  }
  
  folder = new Folder(level, description, fullLink)
  return folder
}

function getLink(target, description, linkData)
{
  var fullLink = ""

  if (linkData != "") {
    fullLink = "'"+linkData+"' onclick='parent.parent.menu.M4SetPageValue("+target+");' target=\""+Var[4]+"\""
  }

  linkItem = new Item(description, fullLink, target, linkData)
  return linkItem
}

function insertFolder(parentFolder, childFolder)
{
  return parentFolder.addChild(childFolder)
}

function insertDoc(parentFolder, document)
{
  return parentFolder.addChild(document)
}


// GLOBAL VARIABLES
// ****************

indexOfEntries = new Array

Var       = new Array // ARRAY OF VARIABLES USED THROUGHOUT RUN
  Var[0]=0            // nEntries
  Var[1]=0            // browser version
  Var[22]=0           // previous node accessed
  Var[23]=0           // current node accessed

//-->
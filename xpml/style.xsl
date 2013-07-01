<!-- $Id: style.xsl,v 1.1 2005/10/12 00:00:49 jnoll Exp $ -->
<!-- CSS declarations for pmldoc (and webpeos). -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">

<xsl:output method="html" indent="yes" />

<xsl:template name="style">
   <STYLE TYPE="text/css">
     TABLE { border: 1px solid white; }
     H1  {  text-transform: uppercase }
     H2 { text-transform: capitalize }
     H3 { text-transform: capitalize }

     scroll {overflow: auto; height: 100px;}


     OL.index { list-style-type: decimal ; 
	  list-style-position: inside; 
     } 
     UL.index { list-style-type: none ; list-style-position: inside} 
     LI.index { text-transform: capitalize; }
   </STYLE>
</xsl:template>

</xsl:stylesheet>

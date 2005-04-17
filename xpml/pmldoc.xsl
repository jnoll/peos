<!-- $Id: pmldoc.xsl,v 1.1 2005/03/04 05:40:34 jnoll Exp $ -->
<!-- Convert XPML to HTML ``Tutorial'' format. -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">

<xsl:output method="html" indent="yes" />

<xsl:template match="/">
 <xsl:apply-templates mode="index"/>
 <xsl:apply-templates mode="detail"/>
</xsl:template>

<xsl:template match="process" mode="index">
 <exsl:document href = "{@name}.html" method ="html" indent="yes">

   <xsl:choose>
    <xsl:when test="boolean(./title)">
     <xsl:call-template name="toc">
      <xsl:with-param name="title" select="title/text()"/>
     </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
     <xsl:call-template name="toc">
     <xsl:with-param name="title" select="translate(@name, '_', ' ')"/>
    </xsl:call-template>
    </xsl:otherwise>
   </xsl:choose>

 </exsl:document>
 
</xsl:template>

<!-- Populate outline page -->
<xsl:template match="process" mode="outline">
    <ol>
     <xsl:apply-templates mode="index"/>
    </ol>
</xsl:template>


<xsl:template match="process" mode="detail">
 <xsl:apply-templates mode="detail"/>
</xsl:template>

<xsl:template match="action" mode="index">
   <li class="index"><a href="{@name}.html"><xsl:value-of select="translate(@name, '_', ' ')"/></a></li>
</xsl:template>

<xsl:template match="iteration" mode="index">
    <li class="index"><xsl:value-of select="translate(@name, '_', ' ')"/>:</li>
    <ol>
     <xsl:apply-templates mode="index"/>
    </ol>
</xsl:template>

<xsl:template match="branch" mode="index">
    <li class="index"><xsl:value-of select="translate(@name, '_', ' ')"/>:</li>
    <ol>
     <xsl:apply-templates mode="index"/>
    </ol>
</xsl:template>

<xsl:template match="selection" mode="index">
    <li class="index"><xsl:value-of select="translate(@name, '_', ' ')"/>:</li>
    <ol>
     <xsl:apply-templates mode="index"/>
    </ol>
</xsl:template>

<xsl:template match="sequence" mode="index">
    <li class="index"><xsl:value-of select="translate(@name, '_', ' ')"/>:</li>
    <ol>
     <xsl:apply-templates mode="index"/>
    </ol>
</xsl:template>

<xsl:template match="action" mode="detail">
 <exsl:document href = "{@name}.html" method ="html" indent="yes">
  <html>
   <head>
   <title><xsl:value-of select="translate(@name, '_', ' ')"/></title>
   <xsl:call-template name="style"/>
   </head>

   <body>
   <xsl:choose>
    <xsl:when test="boolean(/process/title)">
    <xsl:call-template name="banner">
     <xsl:with-param name="title" select="/process/title/text()"/>
    </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
    <xsl:call-template name="banner">
     <xsl:with-param name="title" select="translate(/process/@name, '_', ' ')"/>
    </xsl:call-template>
    </xsl:otherwise>
   </xsl:choose>

    <xsl:call-template name="nav-bar">
     <xsl:with-param name="next" select="following::action/@name"/>
     <xsl:with-param name="prev" select="(preceding::action)[last()]/@name"/>
     <xsl:with-param name="toc" select="/process/@name"/>
    </xsl:call-template>

    <table style="width: 100%;" border="0" cellpadding="2" cellspacing="0">
     <tbody>
      <tr>
      <xsl:call-template name="outline-pane"/>
      <xsl:call-template name="detail-pane"/>
      </tr>
     </tbody>
    </table>

    <xsl:call-template name="nav-bar">
     <xsl:with-param name="next" select="following::action/@name"/>
     <xsl:with-param name="prev" select="(preceding::action)[last()]/@name"/>
     <xsl:with-param name="toc" select="'index'"/>
    </xsl:call-template>
    <xsl:call-template name="footer"/>
   </body>
  </html>
 </exsl:document>
</xsl:template>

<xsl:template match="requires|provides" mode="detail">
 <xsl:value-of select="text()"/>
</xsl:template>

<xsl:template match="script" mode="detail">
 <xsl:apply-templates select="*|text()"/>
</xsl:template>

<xsl:template name="toc">
 <xsl:param name="title"/>
  <html>
   <head>
    <title><xsl:value-of select="$title"/></title>
    <xsl:call-template name="style"/>
   </head>
   <body>
    <xsl:call-template name="banner">
     <xsl:with-param name="title" select="$title"/>
    </xsl:call-template>

    <xsl:call-template name="nav-bar">
     <xsl:with-param name="next" select="child::action/@name"/>
    </xsl:call-template>

    <table style="width: 100%;" border="0" cellpadding="2" cellspacing="0">
     <tbody>
       <tr>
        <td style= "width: 25%; background-color: rgb(204, 204, 255);">
        </td>

	<td>
	
	 <hr/>

	 <h2>Table of Contents</h2>
	 <ol>
	  <xsl:apply-templates mode="index"/>
	 </ol>
	</td>
       </tr>
     </tbody>
    </table>

    <xsl:call-template name="nav-bar">
     <xsl:with-param name="next" select="child::action/@name"/>
    </xsl:call-template>

   </body>
  </html>
</xsl:template>

<xsl:template name="banner">
 <xsl:param name="title" select="'Your Process Name Here'"/>
<table style="text-align: left; background-color: rgb(255, 204, 51); width: 100%;"
  cellspacing="0" cellpadding="1">
  <tbody>
    <tr>
    <td style="width:100px;">
        <img style="width: 100px; height: 100px;" alt="PEOS Logo"
        src="peos_logo.jpg"/>
     </td>
      <td style="vertical-align: middle;">
	<h3><i>COEN 286, Winter 2005</i></h3>
        <h1 style="text-align: center;"><xsl:value-of select="$title"/></h1>
      </td>
    </tr>
  </tbody>
</table>

</xsl:template>

<xsl:template name="outline-pane">
 <xsl:param name="name" select="Process"/>
  <td style=
  "vertical-align: top; width: 25%; background-color: rgb(204, 204, 255);">
        <xsl:apply-templates mode="outline" select="/process"/>
  </td>
</xsl:template>

<xsl:template name="detail-pane">
 <td style="vertical-align: top; text-align: left;">
   <h3><xsl:value-of select="translate(@name, '_', ' ')"/></h3>
   <xsl:if test="boolean(requires)">
   <b>Requires: </b> <xsl:apply-templates select="requires" mode="detail"/><br/>
   </xsl:if>
   <xsl:if test="boolean(provides)">
   <b>Produces: </b> <xsl:apply-templates select="provides" mode="detail"/><br/>
   </xsl:if>
   <xsl:apply-templates select="script" mode="detail"/>
 </td>
</xsl:template>

<xsl:template name="nav-bar">
 <xsl:param name="next"/>
 <xsl:param name="prev"/>
 <xsl:param name="toc"/>

 <table style="text-align: left; background-color: rgb(255, 204, 51);
         width: 100%;"  cellpadding="1" cellspacing="1">
  <tbody>
   <tr>
    <td>
    <xsl:choose>
      <xsl:when test="not(boolean($prev))">
	&lt;&lt;Prev
      </xsl:when>
      <xsl:when test="count($prev) &gt; 0">
	<a href="{$prev}.html">	&lt;&lt;Prev </a>
      </xsl:when>
      <xsl:otherwise>
	&lt;&lt;Prev
      </xsl:otherwise>
    </xsl:choose>
    </td>

    <td>
    <xsl:choose>
      <xsl:when test="not(boolean($next))">
	Next&gt;&gt;
      </xsl:when>
      <xsl:when test="count($next) &gt; 0">
	<a href="{$next}.html">Next&gt;&gt;</a>
      </xsl:when>
      <xsl:otherwise>
	Next&gt;&gt;
      </xsl:otherwise>
    </xsl:choose>
    </td>	

    <xsl:if test="boolean($toc)">
     <td>
     <a href="{$toc}.html">TOC</a>
     </td>
    </xsl:if>
    <td width="100%"></td>
   </tr>
  </tbody>
 </table>
</xsl:template>

<xsl:template name="footer">
  <table style="text-align: left; width: 100%;" 
  cellpadding="1" cellspacing="0">
    <tbody>
      <tr>
        <td style="background-color: rgb(255, 204, 51);">&amp;copy;2004
        Santa Clara University. Please direct your inquiries:
        <a href="mailto:jnoll@scu.edu">jnoll@scu.edu</a></td>
      </tr>
    </tbody>
  </table>
</xsl:template>

<xsl:template name="style">
   <STYLE TYPE="text/css">
     TABLE { border: 1px solid white; }
     H1  {  text-transform: uppercase }
     H2 { text-transform: capitalize }
     H3 { text-transform: capitalize }

     OL { list-style-type: decimal ; list-style-position: inside} 
     UL.index { list-style-type: none ; list-style-position: inside} 
     LI.index { text-transform: capitalize }
   </STYLE>
</xsl:template>

<xsl:template match="title|abstract" mode="index">
</xsl:template>
<xsl:template match="title|abstract" mode="detail">
</xsl:template>

<xsl:template match="em">
 <i><xsl:apply-templates/></i>
</xsl:template>

<xsl:template match="enum">
 <ol>
 <xsl:apply-templates/>
 </ol>
</xsl:template>

<xsl:template match="itemize">
 <ul>
 <xsl:apply-templates/>
 </ul>
</xsl:template>

<xsl:template match="item">
 <li> <xsl:apply-templates/> </li>
</xsl:template>

<xsl:template match="verb|code">
 <pre>
<xsl:apply-templates/>
 </pre>
</xsl:template>

<xsl:template match="url">
 <a href="{@url}"><xsl:value-of select="@name"/></a>
</xsl:template>



<!-- Copy input element to output, so HTML will appear in scripts. -->
<xsl:template match="*">
  <xsl:element name="{name()}">
   <xsl:apply-templates select="*|text()"/>
  </xsl:element>

</xsl:template>


</xsl:stylesheet>
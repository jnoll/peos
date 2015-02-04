<!-- $Id: xpml.xsl,v 1.3 2005/11/28 21:47:13 jnoll Exp $ -->
<!-- Convert XPML to PML -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:strip-space elements="*"/>
<xsl:output method="text" indent="yes" />


<xsl:variable name="newline">
<xsl:text>
</xsl:text>
</xsl:variable>

<xsl:variable name="ket">
<xsl:text> }
</xsl:text>
</xsl:variable>

<xsl:template match="process">
  <xsl:text>process </xsl:text>
  <xsl:value-of select="@name"/>
  <xsl:text> {
</xsl:text>

  <xsl:apply-templates/>

  <xsl:text>}
</xsl:text>
</xsl:template>

<!-- Empty template, to strip out elements for 286 tut-o-matic. -->
<xsl:template match="title|abstract">
</xsl:template>

<xsl:template match="iteration|branch|selection|sequence">
  <xsl:value-of select="name()"/>
  <xsl:text> </xsl:text>
  <xsl:value-of select="@name"/>
  <xsl:text> {
</xsl:text>

  <xsl:apply-templates/>

  <xsl:text>}
</xsl:text>
</xsl:template>


<xsl:template match="action">
  <xsl:text>action </xsl:text>
  <xsl:value-of select="@name"/>
  <xsl:text> {
</xsl:text>

  <xsl:apply-templates/>

  <xsl:text>}
</xsl:text>
</xsl:template>


<xsl:template match="provides|requires">

  <xsl:value-of select="name()"/>
  <xsl:text> { </xsl:text>

  <xsl:value-of select="text()"/>

  <xsl:text> }
</xsl:text>
</xsl:template>


<xsl:template match="script">
  <xsl:text>script { "</xsl:text>

  <xsl:apply-templates select="*|text()"/>

  <xsl:text>" }
</xsl:text>
</xsl:template>


<!-- This BROKEN WINDOW is here because pmldoc uses the 'html' output
     method, while xpml uses 'text.'  So, each tag has to be
     re-implemented to emit html markup as text. -->

<!-- Commonly used LinuxDoc tags.  I find these more convenient than
     their html counterparts, and it's easier to convert old .body
     files to xpml with them. -->

<xsl:template match="em">
 <xsl:text>&lt;i&gt;</xsl:text>
 <xsl:apply-templates/>
 <xsl:text>&lt;/i&gt;</xsl:text>
</xsl:template>

<xsl:template match="enum">
 <xsl:text>&lt;ol&gt;</xsl:text>
 <xsl:apply-templates/>
 <xsl:text>&lt;/ol&gt;</xsl:text>
</xsl:template>

<xsl:template match="itemize">
 <xsl:text>&lt;ul&gt;</xsl:text>
 <xsl:apply-templates/>
 <xsl:text>&lt;/ul&gt;</xsl:text>
</xsl:template>

<xsl:template match="item">
 <xsl:text>&lt;li&gt;</xsl:text>
 <xsl:apply-templates/> 
 <xsl:text>&lt;/li&gt;</xsl:text>
</xsl:template>

<xsl:template match="verb|code">
<xsl:text>&lt;pre&gt;</xsl:text>
<xsl:apply-templates/>
<xsl:text>&lt;/pre&gt;</xsl:text>
</xsl:template>

<xsl:template match="url">
 <xsl:text>&lt;a href='</xsl:text>
 <xsl:value-of select="@url"/><xsl:text>'&gt;</xsl:text>
 <xsl:value-of select="@name"/>
 <xsl:text>&lt;/a&gt;</xsl:text>
</xsl:template>



<!-- Copy input element to output, so HTML will appear in scripts. -->
<xsl:template match="*">
  <xsl:text>&lt;</xsl:text>
  <xsl:value-of select="name()"/>
  <xsl:text>&gt;</xsl:text>
   <xsl:apply-templates select="*|text()"/>
  <xsl:text>&lt;/</xsl:text>
  <xsl:value-of select="name()"/>
  <xsl:text>&gt;</xsl:text>


</xsl:template>

</xsl:stylesheet>


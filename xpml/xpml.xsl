<!-- $Id: xpml.xsl,v 1.2 2005/06/28 22:25:07 jnoll Exp $ -->
<!-- Convert XPML to PML -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:import href="html.xsl"/>

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


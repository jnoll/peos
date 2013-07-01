<!-- $Id: get_model.xsl,v 1.1 2007/04/29 05:08:21 jnoll Exp $ -->
<!-- Print model name for given PID. -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">

<xsl:strip-space elements="*"/>
<xsl:output method="text" indent="no" />

<xsl:template match="process_table">
  <xsl:apply-templates select="process[@pid=$PID]"/>
</xsl:template>

<xsl:template match="process">
  <xsl:value-of select="@model"/>
</xsl:template>

<!-- Capture all remaining forms. -->
<xsl:template match="*">
   <xsl:apply-templates select="*|text()"/>
</xsl:template>


</xsl:stylesheet>

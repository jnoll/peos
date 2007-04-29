<!-- $Id: list_procs.xsl,v 1.2 2007/04/29 17:27:26 jnoll Exp $ -->
<!-- Convert proc_table.dat.xml into html process list with links to instances. -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">

<xsl:output method="html" indent="yes" />
<xsl:strip-space elements="*"/>


<xsl:template match="process_table">
  <xsl:element name="html">
    <xsl:element name="head">
    </xsl:element>
    <xsl:element name="body">
      <xsl:element name="ol">
	<xsl:apply-templates/>
      </xsl:element>
    </xsl:element>
  </xsl:element>
</xsl:template>

<xsl:template match="process">
  <xsl:element name="li">
    <xsl:element name="a">
      <xsl:attribute name="href">
	<xsl:text>update.cgi?PID=</xsl:text><xsl:value-of select="@pid"/>
      </xsl:attribute>
      <xsl:value-of select="@model"/>
    </xsl:element>
  </xsl:element>
</xsl:template>

</xsl:stylesheet>

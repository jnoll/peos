<!-- $Id: list_procs.xsl,v 1.3 2007/04/29 18:54:27 jnoll Exp $ -->
<!-- Convert proc_table.dat.xml into html process list with links to instances. -->
<!-- Output must be inserted into a surrounding body/form element. -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">

<xsl:output method="html" indent="yes" />

<xsl:strip-space elements="*"/>

<xsl:template match="process_table">
      <xsl:element name="ol">
	<xsl:apply-templates/>
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

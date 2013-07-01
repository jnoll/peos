<!-- $Id: html.xsl,v 1.2 2005/06/28 22:25:07 jnoll Exp $ -->
<!-- Conversions for some popular HTML and LinuxDoc tags. -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">

<xsl:output method="html" indent="yes" />

<!-- Commonly used LinuxDoc tags.  I find these more convenient than
     their html counterparts, and it's easier to convert old .body
     files to xpml with them. -->
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

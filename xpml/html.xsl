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
  <xsl:text> &lt;html&gt;
&lt;head&gt;
&lt;title&gt;</xsl:text><xsl:value-of select="@name"/><xsl:text>&lt;/title&gt;
&lt;/head&gt;
&lt;body&gt;
&lt;h1&gt;</xsl:text><xsl:value-of select="@name"/><xsl:text>&lt;/h1&gt;
&lt;ul&gt;
</xsl:text>
  <xsl:apply-templates/>
  <xsl:text>
&lt;/ul&gt;
&lt;/body&gt;
&lt;/html&gt;
</xsl:text>
</xsl:template>


<xsl:template match="iteration">
  <xsl:text>&lt;li&gt;Repeat the following:
&lt;ol&gt;
</xsl:text>

  <xsl:apply-templates/>

  <xsl:text>&lt;/ol&gt;
</xsl:text>
</xsl:template>

<xsl:template match="branch">
  <xsl:text>&lt;li&gt;Do all of the following:
&lt;ul&gt;
</xsl:text>

  <xsl:apply-templates/>

  <xsl:text>&lt;/ul&gt;
</xsl:text>
</xsl:template>

<xsl:template match="selection">
  <xsl:text>&lt;li&gt;Do one of the following:
&lt;ul&gt;
</xsl:text>

  <xsl:apply-templates/>

  <xsl:text>&lt;/ul&gt;
</xsl:text>
</xsl:template>

<xsl:template match="sequence">
  <xsl:text>&lt;li&gt;Do all of the following, in order:
&lt;ol&gt;
</xsl:text>

  <xsl:apply-templates/>

  <xsl:text>&lt;/ol&gt;
</xsl:text>
</xsl:template>


<xsl:template match="action">
  <xsl:text>&lt;li&gt; </xsl:text>
  <xsl:value-of select="@name"/>
  <xsl:text>&lt;br&gt;
</xsl:text>

  <xsl:apply-templates/>

</xsl:template>


<xsl:template match="requires">
  <xsl:text>&lt;bf&gt;Input:&lt;/bf&gt; </xsl:text>
  <xsl:value-of select="text()"/>
  <xsl:text>&lt;br&gt;
</xsl:text>
</xsl:template>

<xsl:template match="provides">
  <xsl:text>&lt;bf&gt;Output:&lt;/bf&gt; </xsl:text>
  <xsl:value-of select="text()"/>
  <xsl:text>&lt;br&gt;
</xsl:text>
</xsl:template>


<xsl:template match="script">
  <xsl:apply-templates select="*|text()"/>
  <xsl:value-of select="$newline"/>
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


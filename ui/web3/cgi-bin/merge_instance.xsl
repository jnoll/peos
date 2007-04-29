<!-- $Id: merge_instance.xsl,v 1.1 2007/04/29 05:08:21 jnoll Exp $ -->
<!-- Insert instance from proc_table.dat.xml into original xpml doc. -->

<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:exsl="http://exslt.org/common"
                extension-element-prefixes="exsl">

<xsl:strip-space elements="*"/>
<xsl:output method="xml" indent="yes" />

<!-- Match xpml elements; these get passed through unchanged. -->
<xsl:template match="document|head|title|subtitle|author|institution|abstract|body|section|process|iteration|branch|selection|sequence">
  <xsl:element name="{name()}">
    <xsl:for-each select="@*">
      <xsl:attribute name="{name()}">
	<xsl:value-of select="."/>
      </xsl:attribute>
    </xsl:for-each>
    <xsl:apply-templates select="*|text()"/>
  </xsl:element>
</xsl:template>

<!-- Process needs a PID attribute. -->
<xsl:template match="process">
  <xsl:element name="{name()}">
    <xsl:for-each select="@*">
      <xsl:attribute name="{name()}">
	<xsl:value-of select="."/>
      </xsl:attribute>
    </xsl:for-each>
    <xsl:attribute name="pid">
      <xsl:value-of select="$PID"/>
    </xsl:attribute>

    <xsl:element name="submit">
    </xsl:element>

    <xsl:apply-templates select="*|text()"/>

    <xsl:element name="submit">
    </xsl:element>
  </xsl:element>
</xsl:template>

<!-- Match action.  This is somewhat difficult since the
     proc table does not store names of control constructs.  So we
     have to lookup each action instead. -->

<xsl:template match="action">
  <xsl:element name="{name()}">
    <xsl:for-each select="@*">
    <!-- emit action's pmldoc attributes. -->
      <xsl:attribute name="{name()}">
	<xsl:value-of select="."/>
      </xsl:attribute>
    </xsl:for-each>
    <!-- Insert action state and resource elements from proc_table.dat.xml. -->
    <xsl:variable name="act_name">
      <xsl:value-of select="@name"/>
    </xsl:variable>
    <xsl:apply-templates select="document('proc_table.dat.xml')/process_table/process[@pid=$PID]//action[@name=$act_name]" mode="proc_table"/>

    <!-- emit action's remaining pmldoc elements. -->
    <xsl:apply-templates/>
  </xsl:element>
</xsl:template>

<!-- Pmldoc action fields that get replaced by proc_table values. -->
<xsl:template match="action/script|requires|provides">
</xsl:template>

<xsl:template match="action" mode="proc_table">
  <xsl:attribute name="state">
    <xsl:value-of select="@state"/>
  </xsl:attribute>
  <xsl:apply-templates mode="proc_table"/>
</xsl:template>

<!-- XXX Ugh.  Once again, resources raise their ugly heads.  How
     should the predicate be shown when a resource has a binding? -->

<xsl:template match="req_resource" mode="proc_table">
  <xsl:element name="requires">
    <xsl:value-of select="@name"/>
    <xsl:text> (</xsl:text>
    <xsl:value-of select="@value"/>
    <xsl:text>)</xsl:text>
  </xsl:element>
</xsl:template>

<xsl:template match="prov_resource" mode="proc_table">
  <xsl:element name="provides">
    <xsl:value-of select="@name"/>
    <xsl:text> (</xsl:text>
    <xsl:value-of select="@value"/>
    <xsl:text>)</xsl:text>
  </xsl:element>
</xsl:template>


<!-- Include the script from proc_table, which has embedded resources. -->
<xsl:template match="script" mode="proc_table">
  <xsl:element name="script"><xsl:apply-templates/> </xsl:element>
</xsl:template>

<!-- Copy remaining input elements to output. -->
<xsl:template match="*">
  <xsl:element name="{name()}">
   <xsl:apply-templates select="*|text()"/>
  </xsl:element>

</xsl:template>


</xsl:stylesheet>

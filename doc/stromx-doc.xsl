<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
    <html>
        <body>
            <xsl:for-each select="Package">
                <h1><xsl:value-of select="@name"/></h1>
                <h2>Data variants</h2>
                <xsl:apply-templates select="DataVariant"/>
                <h2>Operators</h2>
                <xsl:apply-templates select="Operator"/>
            </xsl:for-each>
        </body>
    </html>
</xsl:template>

<xsl:template match="DataVariant">
    <h3><xsl:value-of select="@title"/></h3>
    <p>
        <xsl:value-of select="Description"/>
    </p>
</xsl:template>

<xsl:template match="Operator">
    <h3><xsl:value-of select="@type"/></h3>
    <p>
        <xsl:value-of select="Description"/>
    </p>

    <h4>Parameters</h4>
    <xsl:for-each select="Parameter">
        <p><xsl:value-of select="@title"/></p>
    </xsl:for-each>

    <h4>Inputs</h4>
    <xsl:for-each select="Input">
        <p><xsl:value-of select="@title"/></p>
    </xsl:for-each>

    <h4>Outputs</h4>
    <xsl:for-each select="Output">
        <p><xsl:value-of select="@title"/></p>
    </xsl:for-each>
</xsl:template>

</xsl:stylesheet> 
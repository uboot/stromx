<?xml version="1.0" encoding="ISO-8859-1"?>

<!-- saxon9 -s:package.xml -xsl:stromx-doc.xsl > package.html -->

<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
      <body>
        <xsl:for-each select="Package">
          <xsl:variable name="package" select="@name" />
          <h1>
            <xsl:value-of select="@name"/></h1>
          <h2>Data variants</h2>
          <xsl:for-each select="DataVariant">
            <xsl:variable name="filename" select="concat(@title,'.html')" />
            <p>
            <a>
              <xsl:attribute name="href">
                <xsl:value-of select="$filename"/>
              </xsl:attribute> 
              <xsl:value-of select="@title"/>
            </a>
            </p>
            <xsl:result-document href="{$filename}">
              <html>
                <body>
                  <h1><xsl:value-of select="@title"/></h1>
                  <p>
                    <xsl:value-of select="Description"/>
                  </p>
                </body>
              </html>
            </xsl:result-document>
          </xsl:for-each>
          <h2>Operators</h2>
          <xsl:for-each select="Operator">
            <xsl:variable name="filename" select="concat(@type,'.html')" />
            <p>
            <a>
              <xsl:attribute name="href">
                <xsl:value-of select="$filename"/>
              </xsl:attribute> 
              <xsl:value-of select="@type"/>
            </a>
            </p>
            <xsl:result-document href="{$filename}">
              <html>
                <body>
                  <h1><xsl:value-of select="$package"/>:<xsl:value-of select="@type"/></h1>
                  <p>
                    <xsl:value-of select="Description"/>
                  </p>
                  <h2>Parameters</h2>
                  <xsl:for-each select="Parameter">
                    <p><xsl:value-of select="@title"/></p>
                  </xsl:for-each>

                  <h2>Inputs</h2>
                  <xsl:for-each select="Input">
                    <p><xsl:value-of select="@title"/></p>
                  </xsl:for-each>

                  <h2>Outputs</h2>
                  <xsl:for-each select="Output">
                    <p><xsl:value-of select="@title"/></p>
                  </xsl:for-each>
                </body>
              </html>
            </xsl:result-document>
          </xsl:for-each>
        </xsl:for-each>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet> 
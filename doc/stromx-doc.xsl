<?xml version="1.0" encoding="ISO-8859-1"?>

<!-- cd ../runtime && saxon9 -s:../runtime.xml -xsl:../stromx-doc.xsl > package.html -->
<!-- cd ../example && saxon9 -s:../example.xml -xsl:../stromx-doc.xsl > package.html -->

<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:key name="variantId" match="Variant" use="@id"/>

  <xsl:template match="/">
    <html>
      <body>
        <xsl:for-each select="Package">
          <xsl:variable name="packageTitle" select="@title" />
          <xsl:variable name="packageId" select="@id" />
          <h1>
            <xsl:value-of select="@title"/></h1>
          <xsl:apply-templates select="Description"/>
          <h2>Variants</h2>
          <xsl:for-each select="Variant">
            <xsl:variable name="filename" select="concat('variant_',@id,'.html')" />
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
                  <h1><xsl:value-of select="@title"/>
                    (<a>
                      <xsl:attribute name="href">
                        <xsl:value-of select="concat('../',$packageId,'/package.html')"/>
                      </xsl:attribute>
                      <xsl:value-of select="$packageTitle"/>
                    </a>)
                  </h1>
                  <p>
                    Parent:
                    <xsl:for-each select="Parent">
                      <xsl:apply-templates select="VariantReference"/>
                    </xsl:for-each>
                  </p>
                  <xsl:apply-templates select="Description"/>
                </body>
              </html>
            </xsl:result-document>
          </xsl:for-each>
          <h2>Operators</h2>
          <xsl:for-each select="Operator">
            <xsl:variable name="filename" select="concat('operator_',@type,'.html')"/>
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
                  <h1>
                    <xsl:value-of select="@title"/>
                    (<a>
                      <xsl:attribute name="href">
                        <xsl:value-of select="concat('../',$packageId,'/package.html')"/>
                      </xsl:attribute>
                      <xsl:value-of select="$packageTitle"/>
                    </a>)
                  </h1>
                  <xsl:apply-templates select="Description"/>
                  <h2>Parameters</h2>
                  <xsl:for-each select="Parameter">
                    <h3>
                      <xsl:value-of select="@title"/>
                      (<xsl:apply-templates select="VariantReference"/>)
                    </h3>
                    <xsl:apply-templates select="Description"/>
                    <xsl:apply-templates select="EnumValue"/>
                  </xsl:for-each>

                  <h2>Inputs</h2>
                  <xsl:for-each select="Input">
                    <h3>
                      <xsl:value-of select="@title"/>
                      (<xsl:apply-templates select="VariantReference"/>)
                    </h3>
                    <xsl:apply-templates select="Description"/>
                    <xsl:apply-templates select="EnumValue"/>
                  </xsl:for-each>

                  <h2>Outputs</h2>
                  <xsl:for-each select="Output">
                    <h3>
                      <xsl:value-of select="@title"/>
                      (<xsl:apply-templates select="VariantReference"/>)
                    </h3>
                    <xsl:apply-templates select="Description"/>
                    <xsl:apply-templates select="EnumValue"/>
                  </xsl:for-each>
                </body>
              </html>
            </xsl:result-document>
          </xsl:for-each>
        </xsl:for-each>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="Description">
    <p><xsl:value-of select="."/></p>
  </xsl:template>

  <xsl:template match="EnumValue">
    <h4>
        <xsl:value-of select="@title"/>
    </h4>
    <xsl:apply-templates select="Description"/>
  </xsl:template>

  <xsl:template match="VariantReference">
    <xsl:variable name="variantId" select="@id"/>
    <xsl:variable name="packageId" select="@package"/>
    <xsl:variable name="packageFile" select="concat(@package,'.xml')"/>
    <xsl:for-each select="key('variantId', @id, document($packageFile))">
    <a>
        <xsl:attribute name="href">
        <xsl:value-of select="concat('../',$packageId,'/variant_',$variantId,'.html')"/>
        </xsl:attribute> 
        <xsl:value-of select="@title"/>
    </a>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet> 
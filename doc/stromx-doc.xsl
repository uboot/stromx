<?xml version="1.0" encoding="ISO-8859-1"?>

<!-- cd ../runtime && saxon9 -s:../runtime.xml -xsl:../stromx-doc.xsl > package.html -->
<!-- cd ../example && saxon9 -s:../example.xml -xsl:../stromx-doc.xsl > package.html -->

<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:key name="variantPackageId" match="Package" use="@id"/>
  <xsl:key name="variantId" match="DataVariant" use="@id"/>

  <xsl:template match="/">
    <html>
      <body>
        <xsl:for-each select="Package">
          <xsl:variable name="package" select="@name" />
          <xsl:variable name="packageId" select="@id" />
          <h1>
            <xsl:value-of select="@name"/></h1>
          <h2>Data variants</h2>
          <xsl:for-each select="DataVariant">
            <xsl:variable name="packageFile" select="concat(@variantPackageId,'.xml')"/>
            <xsl:variable name="variantId" select="@variantId"/>
            <xsl:variable name="variantPackageId" select="@variantPackageId"/>
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
                      <xsl:value-of select="$package"/>
                    </a>)
                  </h1>
                  <p>
                    Derived from:
                    <xsl:for-each select="key('variantId', $variantId, document($packageFile))">
                      <a>
                        <xsl:attribute name="href">
                          <xsl:value-of select="concat('../',$variantPackageId,'/variant_',$variantId,'.html')"/>
                        </xsl:attribute> 
                        <xsl:value-of select="@title"/>
                      </a>
                    </xsl:for-each>
                  </p>
                  <p>
                    <xsl:value-of select="Description"/>
                  </p>
                </body>
              </html>
            </xsl:result-document>
          </xsl:for-each>
          <h2>Operators</h2>
          <xsl:for-each select="Operator">
            <xsl:variable name="filename" select="concat('operator_',@id,'.html')"/>
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
                  <h1>
                    <xsl:value-of select="@type"/>
                    (<a>
                      <xsl:attribute name="href">
                        <xsl:value-of select="concat('../',$packageId,'/package.html')"/>
                      </xsl:attribute>
                      <xsl:value-of select="$package"/>
                    </a>)
                  </h1>
                  <p>
                    <xsl:value-of select="Description"/>
                  </p>
                  <h2>Parameters</h2>
                  <xsl:for-each select="Parameter">
                    <h3>
                      <xsl:value-of select="@title"/>
                      <xsl:variable name="packageFile" select="concat(@variantPackageId,'.xml')"/>
                      <xsl:variable name="variantId" select="@variantId"/>
                      <xsl:variable name="variantPackageId" select="@variantPackageId"/>
                      <xsl:for-each select="key('variantId', $variantId, document($packageFile))">
                        (<a>
                          <xsl:attribute name="href">
                            <xsl:value-of select="concat('../',$variantPackageId,'/variant_',$variantId,'.html')"/>
                          </xsl:attribute>
                          <xsl:value-of select="@title"/>
                        </a>)
                      </xsl:for-each>
                    </h3>
                    <p>
                        <xsl:value-of select="Description"/>
                    </p>
                  </xsl:for-each>

                  <h2>Inputs</h2>
                  <xsl:for-each select="Input">
                    <h3>
                      <xsl:value-of select="@title"/>
                      <xsl:variable name="packageFile" select="concat(@variantPackageId,'.xml')"/>
                      <xsl:variable name="variantId" select="@variantId"/>
                      <xsl:variable name="variantPackageId" select="@variantPackageId"/>
                      <xsl:for-each select="key('variantId', $variantId, document($packageFile))">
                        (<a>
                          <xsl:attribute name="href">
                            <xsl:value-of select="concat('../',$variantPackageId,'/variant_',$variantId,'.html')"/>
                          </xsl:attribute>
                          <xsl:value-of select="@title"/>
                        </a>)
                      </xsl:for-each>
                    </h3>
                    <p>
                        <xsl:value-of select="Description"/>
                    </p>
                  </xsl:for-each>

                  <h2>Outputs</h2>
                  <xsl:for-each select="Output">
                    <h3>
                      <xsl:value-of select="@title"/>
                      <xsl:variable name="packageFile" select="concat(@variantPackageId,'.xml')"/>
                      <xsl:variable name="variantId" select="@variantId"/>
                      <xsl:variable name="variantPackageId" select="@variantPackageId"/>
                      <xsl:for-each select="key('variantId', $variantId, document($packageFile))">
                        (<a>
                          <xsl:attribute name="href">
                            <xsl:value-of select="concat('../',$variantPackageId,'/variant_',$variantId,'.html')"/>
                          </xsl:attribute>
                          <xsl:value-of select="@title"/>
                        </a>)
                      </xsl:for-each>
                    </h3>
                    <p>
                        <xsl:value-of select="Description"/>
                    </p>
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
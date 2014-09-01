#include <precompiled.h>
#include <config.h>

Configuration::Configuration( void )
{
    isLoaded = false;
    graphics_config = nullptr;
}

bool Configuration::LoadConfiguration( const std::string &set_filename )
{
    // if the configuration is already loaded, don't do anything. We're fine.
    if( ( filename.compare( set_filename ) == 0 ) && ( isLoaded == true) )
    {
        return true;
    }

    filename = set_filename;
    rapidxml::file<> *xml_file = new rapidxml::file<>( filename.c_str() );
    rapidxml::xml_document<> *xml_document = new rapidxml::xml_document<>();
    xml_document->parse<0>( xml_file->data() );

    if( loadGraphics( xml_document ) == false )
    {
        isLoaded = false;
        return false;
    }
}

std::shared_ptr<GraphicsConfiguration> Configuration::GraphicsConfig( void )
{
    return graphics_config;
}

bool Configuration::loadGraphics( const rapidxml::xml_document<> *xml_document )
{
    if( xml_document == NULL )
    {
        return false;
    }

    rapidxml::xml_node<> *node = xml_document->first_node( "configuration" );
    if( node == NULL )
    {
        return false;
    }

    node = node->first_node( "graphics" );
    if( node == NULL )
    {
        return false;
    }

    std::string renderer;
    bool fullscreen;
    int width;
    int height;
    bool v_sync;

    std::string node_name;
    std::string node_value;

    for( rapidxml::xml_node<> *property_node = node->first_node(); 
         property_node != NULL; 
         property_node = property_node->next_sibling() )
    {
        node_name = property_node->name();
        node_value = property_node->value();
        if( node_name.compare( "fullscreen" ) == 0 )
        {
            if( node_value.compare( "True" ) == 0 )
            {
                fullscreen = true;
            }
            else
            {
                fullscreen = false;
            }
        }

        if( node_name.compare( "renderer" ) == 0 )
        {
            renderer = node_value;
        }

        if( node_name.compare( "width" ) == 0 )
        {
            try
            {
                width = std::stoi( node_value );
            }
            catch( std::invalid_argument )
            {
                width = 800;
            }
        }

        if( node_name.compare( "height" ) == 0 )
        {
            try
            {
                height = std::stoi( node_value );
            }
            catch( std::invalid_argument )
            {
                height = 480;
            }
        }

        if( node_name.compare( "vsync" ) == 0 )
        {
            if( node_value.compare( "True" ) == 0 )
            {
                v_sync = true;
            }
            else
            {
                v_sync = false;
            }
        }
    }

    graphics_config = std::make_shared<GraphicsConfiguration>
                      ( 
                          renderer,
                          fullscreen,
                          v_sync,
                          glm::ivec2(width, height)
                      );
    return true;
}
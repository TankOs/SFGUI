#pragma once

#include <SFGUI/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sfg
{
    /**
    *   \brief An extension of sfg::Image with the ability to resize the image.
    **/
    class SFGUI_API ResizableImage : public Image
    {
    public:
        typedef std::shared_ptr<ResizableImage> Ptr; //!< Shared pointer of ResizableImage
        typedef std::shared_ptr<const ResizableImage> PtrConst; //!< Shared pointer of ResizableImage

        /**
        *   \brief Create new sfg::ResizableImage instance.
        *
        *   You have to create a new instance of ResizableImage by calling this static function.
        *   Since SFGUI keeps tracking of created objects for You, so You don't have to hassle
        *   with \c new and \c delete.
        *
        *   You will receive a shared pointer \c sfg::ResizableImage::Ptr to the new instance of ResizableImage
        *   that You can use to access members of ResizableImage.
        *
        *   \code
        *   sf::Image sfImage;
        *   sfImage.loadFromFile( "image.png" );
        *
        *   // Create new instance and get a shared pointer
        *   sfg::ResizableImage::Ptr resizableImage = sfg::ResizableImage::Create( sfImage );
        *
        *   // Now You can use the shared pointer
        *   resizableImage->Resize( 100, 100 );
        *   \endcode
        *
        *   \param image Source sf::Image. Empty sf::Image if not specified.
        *   \return sfg::ResizableImage::Ptr, a shared pointer to the new instance of ResizableImage.
        */
        static Ptr Create(const sf::Image& image = sf::Image())
        {
            Ptr resizableImagePointer(new ResizableImage(image));
            return resizableImagePointer;
        }

        /**
        *   \brief Get the Widget's name.
        *
        *   Used by rendering engine to apply CSS like style to the Widget.
        *
        *   \return "ResizableImage" as std::string
        */
        virtual const std::string& GetName() const
        {
            static const std::string name("ResizableImage");
            return name;
        }

        /**
        *   \brief Dtor.
        **/
        ~ResizableImage()
        {}

        /**
        *   \brief Set the original sf::Image that will be used for resizing.
        *
        *   It overwrites the old sf::Image.
        *   If You want to resize the image, then You need to call Resize again,
        *   otherwise the image will be displayed in full size.
        *
        *   \param image New sf::Image
        *
        *   \code
        *   sf::Image newImage;
        *   newImage.loadFromFile( "newImage.png" );
        *
        *   resizableImage->SetImage( newImage );
        *   resizableImage->Resize( 100, 100 ); // Don't forget to call if You want it resized
        *   \endcode
        *
        *   \see GetImage, GetResizedImage
        **/
        void SetImage(const sf::Image& image)
        {
            this->m_OriginalImage = image;
            Image::SetImage(this->m_OriginalImage);
        }

        /**
        *   \brief Get the associated sf::Image.
        *
        *   \return The original full size sf::Image used for resizing.
        *
        *   \code
        *   // Get the original full size image
        *   sf::Image sfImage = resizableImage->GetImage();
        *   \endcode
        *
        *   \see GetResizedImage, SetImage
        **/
        const sf::Image& GetImage() const
        {
            return m_OriginalImage;
        }

        /**
        *   \brief Get the generated resized sf::Image.
        *
        *   You can get the generated resized sf::Image by calling
        *   GetResizedImage. If the Resize wasn't called, the
        *   full size sf::Image will be returned, since You have
        *   to call Resize first to generate the new resized sf::Image.
        *
        *   \return The generated resized sf::Image, if Resize was called.
        *
        *   \code
        *   resizableImage->Resize( 100, 100 ); // Call Resize to generate the image
        *   sf::Image resizedImage = resizableImage->GetResizedImage();
        *   \endcode
        *
        *   \see GetImage, SetImage
        **/
        const sf::Image& GetResizedImage() const
        {
            return Image::GetImage();
        }

        /** Resize the image to a new size.
         *  \param sizeX unsigned int
         *  \param sizeY unsigned int
         */
        void Resize(const unsigned int sizeX, const unsigned int sizeY)
        {
            this->Resize(sf::Vector2u(sizeX, sizeY));
        }

        /**
        *   \brief Resize the image to a new size.
        *
        *   Resize generates new sf::Image that will have the requested size. If no size is
        *   given (or null size), original image size will be used.
        *   Original sf::Image is _never_ modified. New sf::Image is created when resizing.
        *
        *   By default, aspect of the image is kept when resizing. Image is then centered inside
        *   the specified area. If You want to stretch the image instead, You can call
        *   SetKeepAspect with true parameter to change how the image will be generated.
        *
        *   You _need_ to call Resize to generate new image.
        *
        *   Resized image can be then retrieved by calling GetResizedImage.
        *
        *   \param customSize sf::Vector2u ( sizeX, sizeY ), if not specified, default ( 0, 0 )
        *
        *   \see GetImage, GetResizedImage, SetImage, SetKeepAspect, GetKeepAspect
        */
        void Resize(const sf::Vector2u& customSize = sf::Vector2u(0, 0))
        {
            // If no size is given (0,0) , use original image...
            if (!customSize.x || !customSize.y)
            {
                Image::SetImage(m_OriginalImage);
                return;
            }

            // Let the flood begin
            float originalX = static_cast<float>(m_OriginalImage.getSize().x);
            float originalY = static_cast<float>(m_OriginalImage.getSize().y);
            float desiredX = static_cast<float>(customSize.x);
            float desiredY = static_cast<float>(customSize.y);

            // Compute the scale we will apply to image
            float scaleX = desiredX / originalX;
            float scaleY = desiredY / originalY;

            // Get texture of the original image
            sf::Texture tempTexture;
            tempTexture.loadFromImage(m_OriginalImage);

            // Sprite allows transformations to be made to it, so let's assign our texture to it
            //      (warn: sprite doesn't own the texture, keep it alive till we're done)
            sf::Sprite tempSprite;
            tempSprite.setTexture(tempTexture, true);


            if (m_KeepAspect)
            {
                // Use same scale for both sides of the sprite
                float lowerScale = std::min(scaleX, scaleY);

                tempSprite.scale(lowerScale, lowerScale);


                // Since the image will not be stretched out because we want to keep the aspect of the
                //  image, there will be an empty space to the right/down of the resized image. I think
                //  it's better if the empty place is distributed around the resized image on all sides,
                //  so we move the sprite to the center of the rectangle we set by size we want.
                //
                // Example
                //           ###                                ###00
                //           ###                                ###00
                //           ###                                ###00
                //           ###                                ###00
                // image 5x3 ###  resize it to 5x5, keep aspect ###00 0=empty space only on one side
                //
                //
                //                     0###0
                //                     0###0
                //                     0###0
                //                     0###0
                // so center the image 0###0 and now empty space is distributed to both sides

                // Move the sprite to the center of the resized image
                float offsetX = (desiredX - (originalX * (lowerScale))) / 2;
                float offsetY = (desiredY - (originalY * (lowerScale))) / 2;

                tempSprite.move(offsetX, offsetY);
            }
            else
            {
                tempSprite.scale(scaleX, scaleY);
            }

            // Transformations on Sprite are set, so we can pre-render the sprite on
            //  a new texture with a transparent background
            sf::RenderTexture tempRenderTexture;
            tempRenderTexture.create(customSize.x, customSize.y);
            tempRenderTexture.setSmooth(true);
            tempRenderTexture.clear(sf::Color(255, 255, 255, 0));
            tempRenderTexture.draw(tempSprite);
            tempRenderTexture.display();

            // Now when we have the new texture with our resized image ready
            //  we can set it as a new image to the underlying base Image
            //  class, and we are done.
            Image::SetImage(tempRenderTexture.getTexture().copyToImage());
        }

        /**
        *   \brief Should aspect of image be kept when resizing of image?
        *
        *   Set true, if aspect should be kept, or false if image should be stretched to given size.
        *   You should set this before calling Resize.
        *
        *   \param value bool value, true if You want to keep aspect, false if You want the image stretched.
        *
        *   \code
        *   // Set You want to stretch the image, and remember to call Resize
        *   resizableImage->SetKeepAspect( false );
        *   resizableImage->Resize( 100, 100 );
        *   \endcode
        *
        *   \see GetKeepAspect, Resize
        **/
        void SetKeepAspect(const bool& value = true)
        {
            this->m_KeepAspect = value;
        }

        /**
        *   \brief Get whether aspect of image is kept when resizing of image.
        *
        *   \param True if aspect of image is kept, false if image is stretched.
        *
        *   \see SetKeepAspect, Resize
        **/
        const bool GetKeepAspect() const
        {
            return this->m_KeepAspect;
        }

    protected:
        /**
        *   \brief Construct sfg::ResizableImage from a source sf::Image.
        *
        *   We want to disallow instantiation.
        *   SFGUI keeps track of it's objects with shared pointers.
        *
        *   \param image Existing sf::Image, or if not specified then default empty sf::Image.
        **/
        ResizableImage(const sf::Image& image = sf::Image())
        {
            m_OriginalImage = image;
            this->SetImage(image);
        }

    private:
        sf::Image m_OriginalImage;      //!< Original unmodified sf::Image.
        bool m_KeepAspect = { true };   //!< Whether we want to keep aspect of image when resizing.

    }; // class ResizableImage
} // namespace sfg

/**
*   \class sfg::ResizableImage
*
*   sfg::ResizableImage is a class that allows to easily resize
*   an image and display it.
*
*   It inherits all the functions from sfg::Image and adds property
*   to specify if You want to keep aspect of an image, when resizing
*   the image, and function to perform the resizing of the image.
*
*   Resizing of the image is done only when the Resize function is
*   called. If Resize function isn't called, the image will remain
*   in it's original size.
*
*   The sf::Image You assign to the sfg::ResizableImage is _never_
*   modified. When You perform resizing, a new sf::Image is created
*   and displayed. You can retrieve the resized image with
*   GetResizedImage function. GetImage will return the original image.
*
*   Please note that the resize is done with a really simple method
*   of applying transformations in OpenGL. Don't expect great quality.
*   If You need great quality, You may need to implement better algorithm
*   or use sf::Shader.
*
*   Resizing of a large image or alot of images can be lengthy. Use
*   proper caching method to speed up the loading of Your images.
*   If You want to save the image after resizing, You can use the
*   GetResizedImage().saveToFile( fileName ) function.
*
*   Usage example:
*   \code
*   // Declare and load sf::Image
*   sf::Image sfImage;
*   sfImage.loadFromFile( "image.png" ); // 132x68 px size
*
*   // Get shared pointer of new sfg::ResizableImage
*   sfg::ResizableImage::Ptr image = sfg::ResizableImage::Create( sfImage );
*
*   // If You don't want to keep the aspect of the image, You may set false. Default is true.
*   // image->SetKeepAspect( false );
*
*   // Perform resizing
*   image->Resize( 100, 100 );
*
*   // Add it to Your layout
*   box->Pack( image );
*   window->Add( box );
*   desktop.Add( window );
*
*   // Draw
*   desktop.Update( .0f ); // You shouldn't pass 0.0 to update function.
*   m_SFGUI.Display( renderWindow );
*   \endcode
*
*   \see sfg::Image, sf::Image
**/

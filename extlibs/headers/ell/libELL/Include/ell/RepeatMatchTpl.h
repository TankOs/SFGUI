// This file is part of Ell library.
//
// Ell library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Ell library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Ell library.  If not, see <http://www.gnu.org/licenses/>.

#if ! defined(MIN) || ! defined(MAX)
# error "MIN and MAX parameters must be defined before inclusion"
#else
        template <typename V>
        bool parse(Parser<Token> * parser, Storage<V> & s) const
        {
            ELL_BEGIN_PARSE
            typename Parser<Token>::Context sav_pos(parser);
            int count = 0;
            typename Storage<V>::Unit se;

            while (count < MIN)
            {
                if (! target.parse(parser, se))
                {
                    sav_pos.restore(parser);
                    break;
                }

                s.enqueue(se);
                ++count;
                parser->skip();
            }

            if (count == MIN)
            {
                if (MAX == -1)
                {
                    match = true;
                    while (target.parse(parser, se))
                    {
                        s.enqueue(se);
                        parser->skip();
                    }
                }
                else
                {
                    if (count <= MAX)
                        match = true;

                    while (count < MAX)
                    {
                        if (! target.parse(parser))
                            break;

                        s.enqueue(se);
                        parser->skip();
                        ++count;
                    }
                }
            }
            ELL_END_PARSE
        }
# undef MIN
# undef MAX
#endif
